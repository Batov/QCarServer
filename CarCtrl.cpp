#include <QtCore/QStringList>
#include <QtNetwork/QTcpSocket>
#include <QtGui/QKeyEvent>

#include <unistd.h>
#include <linux/input.h>


#include "CarCtrl.hpp"

CarCtrl::CarCtrl() :
	c_Server(),
	c_sides(),
	c_StopFlag(0)
{
	initSettings();
	printf("ConnectionPort = %d\n", c_settings->value("ConnectionPort").toInt());
	initSides();

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

	c_defaultSettings->beginGroup("SidesKeys");
	c_defaultSettings->setValue("left", 0);
	c_defaultSettings->setValue("right", 1);
	c_defaultSettings->endGroup();

	c_defaultSettings->beginGroup("left"); 
	c_defaultSettings->setValue("CentralWheel", 3 );
	c_defaultSettings->setValue("EdgesWheels", 1);
	c_defaultSettings->setValue("Period", 20000 );
	c_defaultSettings->endGroup();

	c_defaultSettings->beginGroup("right");
	c_defaultSettings->setValue("CentralWheel", 2 );
	c_defaultSettings->setValue("EdgesWheels", 3);
	c_defaultSettings->setValue("Period", 20000 );
	c_defaultSettings->endGroup();

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

void CarCtrl::initSides()
{
	c_settings->beginGroup("SidesKeys");
	QStringList sidesKeys = c_settings->allKeys();
	c_settings->endGroup();

	QString DevPath  = c_settings->value("DevPath").toString();
	int DevId = c_settings->value("DevId").toInt();
	I2cConnection* i2cCon = new I2cConnection(DevPath,DevId); //one i2c connection for everybody
	c_i2cCon = i2cCon;

	for (int i = 0; i < sidesKeys.size(); ++i) 
	{
		QString name = sidesKeys[i];
		c_settings->beginGroup(name);
		char Center = (char) (c_settings->value("CentralWheel").toInt());
		char Edge = (char) c_settings->value("EdgesWheels").toInt();
		int Period = c_settings->value("Period").toInt();
		Side* side = new Side(Center,Edge,Period,i2cCon);
		c_sides[name] = side;
		c_settings->endGroup();
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
	Side* side;
	foreach (side, c_sides) 
	{
		side->setPower(0);
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

	QString commandName = cmd.at(0).trimmed();
	if (commandName == "pad")
	{
		if (cmd.at(1).trimmed().toInt() == 1)
		{
			if (cmd.at(2).trimmed() == "up") 
				Stop();
			else 
				{
					int x = cmd.at(2).trimmed().toInt();
					int y =	cmd.at(3).trimmed().toInt();
					int right =  y - x;  //check and fix it
					int left = y + x;
					if (c_StopFlag == 0)
					{
						c_sides["left"]->setPower(left);
						c_sides["right"]->setPower(right);
					}
				}
		}
	}
	else if (commandName == "btn")
			{ 
				if (cmd.at(1).trimmed().toInt() == 1)
					if (c_StopFlag == 1) ResumeMoving();
					else EmergencyStop();
			}
	else if (commandName == "wheel")
			{
				int left = 80 + cmd.at(1).trimmed().toInt(); // check and fix it
				int right = 80 - cmd.at(1).trimmed().toInt();
				if (c_StopFlag == 0)
					{
						c_sides["left"]->setPower(left);
						c_sides["right"]->setPower(right);
					}
			}
	else
		{
			qDebug() << "Unknown command" ;
		}
		//qDebug() << "Request" << command;
	
}