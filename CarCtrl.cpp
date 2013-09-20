#include <QtCore/QStringList>
#include <QtNetwork/QTcpSocket>
#include <QtGui/QKeyEvent>

#include <unistd.h>
#include <linux/input.h>


#include "CarCtrl.hpp"

CarCtrl::CarCtrl() :
	c_Server(),
	c_motors(),
	c_StopFlag(0)
{
	initSettings();
	printf("ConnectionPort = %d\n", c_settings->value("ConnectionPort").toInt());
	initMotors();

	c_Connection = new QTcpSocket();
	c_Server.listen(QHostAddress::Any, c_settings->value("ConnectionPort").toInt());
	connect(&c_Server, SIGNAL(newConnection()), this, SLOT(Connection()));
}

CarCtrl::~CarCtrl()
{
	EmergencyStop();	
	Disconnected(); 
	c_i2cCon->CloseConnection();
}

void CarCtrl::initSettings()
{
	c_defaultSettings = new QSettings(this);

	// default settings init

	c_defaultSettings->beginGroup("MotorsKeys");
	c_defaultSettings->setValue("First", 1);
	c_defaultSettings->setValue("Second", 2);
	c_defaultSettings->setValue("Third", 3);
	c_defaultSettings->setValue("Fourth", 4);
	c_defaultSettings->endGroup();

	c_defaultSettings->setValue("Period", 20000);

	c_defaultSettings->setValue("DevPath", "/dev/i2c-2");
	c_defaultSettings->setValue("DevId", 72); //0x48

	c_defaultSettings->setValue("ConnectionPort", 4444);

	c_settings = new QSettings(QApplication::applicationDirPath() + "/config.ini", QSettings::IniFormat);

	if (c_settings->allKeys().count() == 0) 
	{
		QStringList keys = c_defaultSettings->allKeys();
		for (unsigned int i = 0; i < keys.count(); ++i) 
		{
			c_settings->setValue(keys.at(i), c_defaultSettings->value(keys.at(i)));
		}
	}

	c_settings->setFallbacksEnabled(false);
	c_settings->sync();
}

void CarCtrl::initMotors()
{
	c_settings->beginGroup("MotorsKeys");
	QStringList motorsKeys = c_settings->allKeys();
	c_settings->endGroup();

	QString DevPath  = c_settings->value("DevPath").toString();
	int DevId = c_settings->value("DevId").toInt();
	I2cConnection* i2cCon = new I2cConnection(DevPath,DevId); //one i2c connection for everybody
	c_i2cCon = i2cCon;

	for (int i = 1; i <= motorsKeys.size(); ++i) 
	{
		QString name = motorsKeys[i];
		int Period = c_settings->value("Period").toInt();
		Motor* motor = new Motor(i,Period,i2cCon);
		c_motors[name] = motor;
	}

}

void CarCtrl::ResumeMoving()
{
	c_StopFlag = 0;
}
void CarCtrl::EmergencyStop()
{
	c_StopFlag = 1;
	Stop();
}
void CarCtrl::Stop()
{
	Motor* motor;
	foreach (motor, c_motors) 
	{
		motor->setPower(0);
	}
}

void CarCtrl::Connection()
{
	Stop();
	if (c_Connection->isValid()) qDebug() << "Replacing existing connection";
	c_Connection = c_Server.nextPendingConnection();
	qDebug() << "Accepted new connection";
	c_Connection->setSocketOption(QAbstractSocket::LowDelayOption, 1);
	connect(c_Connection, SIGNAL(disconnected()), this, SLOT(Disconnected()));
	connect(c_Connection, SIGNAL(readyRead()), this, SLOT(NetworkRead()));
}

void CarCtrl::Disconnected()
{
	Stop();
	qDebug() << "Disconnected, STOP MOTORS!";
	c_Connection->disconnectFromHost();
}

void CarCtrl::Responce(const QByteArray& a)
{
	c_Connection->write(a);
}

void CarCtrl::NetworkRead()
{
	if (!c_Connection->isValid()) return;

	while (c_Connection->bytesAvailable() > 0)
	{
		char data[100];
		c_Connection->readLine(data, 100);
		QString command(data);
		Run(command.split(" ", QString::SkipEmptyParts));
	}
		
}

void CarCtrl::keyPressEvent(QKeyEvent* event) 
{	
    printf("\nkey event from board: %d", event->key());
    qDebug() << "Pressed";
    if (c_StopFlag == 0) EmergencyStop(); else ResumeMoving(); 
}


void CarCtrl::Run(QStringList cmd)
{
	qDebug() << cmd;

	c_motors["First"]->setPower(-100);
	c_motors["Second"]->setPower(100);
	c_motors["Third"]->setPower(-100);
	c_motors["Fourth"]->setPower(100);
	
	// QString commandName = cmd.at(0).trimmed();
	// if (commandName == "pad")
	// {
	// 	if (cmd.at(1).trimmed().toInt() == 1)
	// 	{
	// 		if (cmd.at(2).trimmed() == "up") 
	// 			Stop();
	// 		else 
	// 			{
					
	// 			}
	// 	}
	// }
	// else if (commandName == "btn")
	// 		{ 
	// 			if (cmd.at(1).trimmed().toInt() == 1)
	// 				if (c_StopFlag == 1) ResumeMoving();
	// 				else EmergencyStop();
				
	// 		}
	// else if (commandName == "wheel")
	// 		{
				
	// 		}
	// else
	// 	{
	// 		qDebug() << "Unknown command" ;
	// 	}
	
}