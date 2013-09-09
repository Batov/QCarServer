#include <QtCore/QStringList>
#include <QtNetwork/QTcpSocket>

#include <cmath>
#include "CarCtrl.hpp"
#include "Sensor.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

CarCtrl::CarCtrl() :
	m_qrealServer(),
	m_sides(),
	m_timerId(-1)
{
	initSettings();
	initSides();

	m_qrealConnection = new QTcpSocket();
	m_qrealServer.listen(QHostAddress::Any, m_settings->value("ConnectionPort").toInt());
	connect(&m_qrealServer, SIGNAL(newConnection()), this, SLOT(onQRealConnection()));
}

void CarCtrl::initSettings()
{
	m_defaultSettings = new QSettings(this);

	// default settings init
	m_defaultSettings->beginGroup("SidesKeys");
	m_defaultSettings->setValue("left", 0);
	m_defaultSettings->setValue("right", 1);
	m_defaultSettings->endGroup();

	m_defaultSettings->beginGroup("left"); 
	m_defaultSettings->setValue("CentralWheel", 1 );
	m_defaultSettings->setValue("EdgesWheels", 2);
	m_defaultSettings->setValue("Period", 20000 );
	m_defaultSettings->endGroup();

	m_defaultSettings->beginGroup("right");
	m_defaultSettings->setValue("CentralWheel", 3 );
	m_defaultSettings->setValue("EdgesWheels", 4);
	m_defaultSettings->setValue("Period", 20000 );
	m_defaultSettings->endGroup();

	m_defaultSettings->beginGroup("i2cConnection");
	m_defaultSettings->setValue("DevPath", "/dev/i2c-2");
	m_defaultSettings->setValue("DevId", 0x48);
	m_defaultSettings->endGroup();

	m_defaultSettings->setValue("ConnectionPort", 4444);

	m_settings = new QSettings(QApplication::applicationDirPath() + "/config.ini", QSettings::IniFormat);

	if (m_settings->allKeys().count() == 0) {
		// use default
		QStringList keys = m_defaultSettings->allKeys();
		for (unsigned int i = 0; i < keys.count(); ++i) {
			m_settings->setValue(keys.at(i), m_defaultSettings->value(keys.at(i)));
		}
	}

	m_settings->setFallbacksEnabled(false);
	m_settings->sync();
}

void CarCtrl::initSides()
{
	m_settings->beginGroup("SidesKeys");
	QStringList sidesKeys = m_settings->allKeys();
	m_settings->endGroup();

	for (int i = 0; i < sidesKeys.size(); ++i) 
	{
		QString name = sidesKeys[i];
		m_settings->beginGroup(name);
		char Center = m_settings->value("CentralWheel").toChar().toAscii();
		char Edge = m_settings->value("EdgesWheels").toChar().toAscii();
		int Period = m_settings->value("Period").toInt();
		QString DevPath  = m_settings->value("DevPath");
		int DevId = m_settings->value("DevId").toInt();
		i2cConnection* i2cCon = new i2cConnection(DevPath,DevId);
		Side* side = new Side(Center,Edge,Period,i2cCon);
		m_settings->endGroup();
	}

}

void CarCtrl::emergencyStop()
{
	CarMotor* sides;
	foreach (side, m_sides) {
		side->setPower(0);
	}
}

void CarCtrl::onQRealConnection()
{
	if (m_qrealConnection->isValid())
		qDebug() << "Replacing existing QReal connection";
	m_qrealConnection = m_qrealServer.nextPendingConnection();
	qDebug() << "Accepted new QReal connection";
	m_qrealConnection->setSocketOption(QAbstractSocket::LowDelayOption, 1);
	connect(m_qrealConnection, SIGNAL(disconnected()), this, SLOT(onQRealDisconnected()));
	connect(m_qrealConnection, SIGNAL(readyRead()), this, SLOT(onQRealNetworkRead()));
}

void CarCtrl::onQRealDisconnected()
{
	qDebug() << "Existing QReal connection disconnected";
	m_qrealConnection->disconnectFromHost();
}

void CarCtrl::qrealResponce(const QByteArray& a)
{
	m_qrealConnection->write(a);
}

void CarCtrl::onQRealNetworkRead()
{
	if (!m_qrealConnection->isValid())
		return;

	while (m_qrealConnection->bytesAvailable() > 0)
	{
		char data[100];
		m_qrealConnection->readLine(data, 100);
		QString command(data);
		QStringList cmd = command.split(" ", QString::SkipEmptyParts);

		QString commandName = cmd.at(0).trimmed();
		if (m_motors.contains(commandName)) {
			if (!m_autopilot){
				m_motors[commandName]->invoke(cmd.at(1).toInt());
			}
		}
		else if (m_sensors.contains(commandName)) {
			if (!m_autopilot){
				qrealResponce(m_sensors[commandName]->getByteValue());
			}
		}
		else if (commandName == "sound" || commandName == "beep") {
			m_autopilot = !m_autopilot;
			qDebug() << "Unknown command: ";
			if (m_autopilot){
				m_timerId = startTimer(20);

			}
			else 
			{
				killTimer(m_timerId);
				m_timerId =-1;
				this->emergencyStop();
			}
			//	playSound(m_settings->value("SoundFile").toString());
		}
		else {
			qDebug() << "Unknown command: " + cmd.at(0);
		}
		qDebug() << "QReal request " << command;
	}
}
