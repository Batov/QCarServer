#include <QtCore/QStringList>
#include <QtNetwork/QTcpSocket>

#include <cmath>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

#include "CarCtrl.hpp"
#include "I2cConnection.hpp"

CarCtrl::CarCtrl() :
	c_Server(),
	c_sides(),
	c_timerId(-1)
{
	initSettings();
	initSides();

	c_Connection = new QTcpSocket();
	c_Server.listen(QHostAddress::Any, c_settings->value("ConnectionPort").toInt());
	connect(&c_Server, SIGNAL(newConnection()), this, SLOT(Connection()));
}

void CarCtrl::initSettings()
{
	c_defaultSettings = new QSettings(this);

	// default settings init
	c_defaultSettings->beginGroup("SidesKeys");
	c_defaultSettings->setValue("left", 0);
	c_defaultSettings->setValue("right", 1);
	c_defaultSettings->endGroup();

	c_defaultSettings->beginGroup("left"); 
	c_defaultSettings->setValue("CentralWheel", 1 );
	c_defaultSettings->setValue("EdgesWheels", 2);
	c_defaultSettings->setValue("Period", 20000 );
	c_defaultSettings->endGroup();

	c_defaultSettings->beginGroup("right");
	c_defaultSettings->setValue("CentralWheel", 3 );
	c_defaultSettings->setValue("EdgesWheels", 4);
	c_defaultSettings->setValue("Period", 20000 );
	c_defaultSettings->endGroup();

	c_defaultSettings->setValue("DevPath", "/dev/i2c-2");
	c_defaultSettings->setValue("DevId", 0x48);

	c_defaultSettings->setValue("ConnectionPort", 4444);

	c_settings = new QSettings(QApplication::applicationDirPath() + "/config.ini", QSettings::IniFormat);

	if (c_settings->allKeys().count() == 0) {
		// use default
		QStringList keys = c_defaultSettings->allKeys();
		for (unsigned int i = 0; i < keys.count(); ++i) {
			c_settings->setValue(keys.at(i), c_defaultSettings->value(keys.at(i)));
		}
	}

	c_settings->setFallbacksEnabled(false);
	c_settings->sync();
}

void CarCtrl::initSides()
{
	c_settings->beginGroup("SidesKeys");
	QStringList sidesKeys = c_settings->allKeys();
	c_settings->endGroup();

	for (int i = 0; i < sidesKeys.size(); ++i) 
	{
		QString name = sidesKeys[i];

		c_settings->beginGroup(name);
		char Center = c_settings->value("CentralWheel").toChar().toAscii();
		char Edge = c_settings->value("EdgesWheels").toChar().toAscii();
		int Period = c_settings->value("Period").toInt();
		c_settings->endGroup();

		QString DevPath  = c_settings->value("DevPath").toString();
		int DevId = c_settings->value("DevId").toInt();
		I2cConnection* i2cCon = new I2cConnection(DevPath,DevId);
		Side* side = new Side(Center,Edge,Period,i2cCon);
		
	}

}

void CarCtrl::emergencyStop()
{
	Side* side;
	foreach (side, c_sides) 
	{
		side->setPower(0);
	}
}

void CarCtrl::Connection()
{
	if (c_Connection->isValid())
		qDebug() << "Replacing existing connection";
	c_Connection = c_Server.nextPendingConnection();
	qDebug() << "Accepted new connection";
	c_Connection->setSocketOption(QAbstractSocket::LowDelayOption, 1);
	connect(c_Connection, SIGNAL(disconnected()), this, SLOT(Disconnected()));
	connect(c_Connection, SIGNAL(readyRead()), this, SLOT(NetworkRead()));
}

void CarCtrl::Disconnected()
{
	qDebug() << "Disconnected";
	c_Connection->disconnectFromHost();
}

void CarCtrl::Responce(const QByteArray& a)
{
	c_Connection->write(a);
}

void CarCtrl::NetworkRead()
{
	if (!c_Connection->isValid())
		return;

	while (c_Connection->bytesAvailable() > 0)
	{
		char data[100];
		c_Connection->readLine(data, 100);
		QString command(data);
		QStringList cmd = command.split(" ", QString::SkipEmptyParts);

		QString commandName = cmd.at(0).trimmed();

		if (c_sides.contains(commandName)) 
		{
			c_sides[commandName]->setPower(cmd.at(1).toInt());
		}
		//what the fuck?
		// else if (commandName == "sound" || commandName == "beep") {
		// 	m_autopilot = !m_autopilot;
		// 	qDebug() << "Unknown command: ";
		// 	if (m_autopilot){
		// 		m_timerId = startTimer(20);

		// 	}
		// 	else 
		// 	{
		// 		killTimer(m_timerId);
		// 		m_timerId =-1;
		// 		this->emergencyStop();
		// 	}
		// 	//	playSound(m_settings->value("SoundFile").toString());
		//}
		else {
			qDebug() << "Unknown command: " + cmd.at(0);
		}
		qDebug() << "Request " << command;
	}
}
