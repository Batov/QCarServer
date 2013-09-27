#include <QtCore/QStringList>
#include <QtNetwork/QTcpSocket>
#include <QtGui/QKeyEvent>
#include <QTimer>

#include <unistd.h>
#include <linux/input.h>


#include "GunCtrl.hpp"

//const int zeroPoint = 1505000;
//const int amplitude = 200000;
//int setDutyNs(int nspeed){return zeroPoint + (nspeed * amplitude) / 100;}

GunCtrl::GunCtrl() :
	c_Server(),
	lift(),
	shot(),
	reload(),
    PlayFlag(0)
{
	initSettings();
	printf("ConnectionPort = %d\n", c_settings->value("ConnectionPort").toInt());
	initMotor();
	initServo();

	c_Connection = new QTcpSocket();
	c_Server.listen(QHostAddress::Any, c_settings->value("ConnectionPort").toInt());
	connect(&c_Server, SIGNAL(newConnection()), this, SLOT(Connection()));
}

GunCtrl::~GunCtrl()
{	
	Disconnected(); 
	c_i2cCon->CloseConnection();
    run->write("0");
    run->close();
    period_ns->close();
    duty_ns->close();
    request->write("0");
    request->close();
}

void GunCtrl::initSettings()
{
	c_defaultSettings = new QSettings(this);

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

void GunCtrl::initMotor()
{

	QString DevPath  = c_settings->value("DevPath").toString();
	int DevId = c_settings->value("DevId").toInt();
	I2cConnection* i2cCon = new I2cConnection(DevPath,DevId); //one i2c connection for everybody
	c_i2cCon = i2cCon;

	Motor* thelift = new Motor(1,20000,i2cCon);
	lift = thelift;

	Motor* theshot = new Motor(2,20000,i2cCon);
	shot = theshot;

	Motor* therel = new Motor(3,20000,i2cCon);
	reload = therel;
}

void GunCtrl::initServo()
{
	path = new QString ("/sys/class/pwm/ehrpwm.1:1/" );
	request = new QFile (*path + QString("request"));
	if(!request->open(QFile::WriteOnly))
        {
            qDebug() << "Cannot open request #" ;
        }
    if(request->write("1") == -1 || !request->flush())
        {
            qDebug() << "Permission is not given to engine #" ;
        }
	period_ns = new QFile (*path + QString("period_ns"), this);
    duty_ns = new QFile (*path + QString("duty_ns"), this);
    run = new QFile (*path + QString("run"), this);
    if(!(period_ns->open(QFile::WriteOnly) && duty_ns->open(QFile::WriteOnly) && run->open(QFile::WriteOnly)))
        {
            qDebug() << "Cannot open either run or duty or period #";;
        }
    period_ns->write("20000000");
    period_ns->flush();
}

void GunCtrl::Connection()
{
	if (c_Connection->isValid()) qDebug() << "Replacing existing connection";
	c_Connection = c_Server.nextPendingConnection();
	qDebug() << "Accepted new connection";
	c_Connection->setSocketOption(QAbstractSocket::LowDelayOption, 1);
	connect(c_Connection, SIGNAL(disconnected()), this, SLOT(Disconnected()));
	connect(c_Connection, SIGNAL(readyRead()), this, SLOT(NetworkRead()));
}

void GunCtrl::Disconnected()
{
	qDebug() << "Disconnected, STOP MOTORS!";
	c_Connection->disconnectFromHost();
}

void GunCtrl::Responce(const QByteArray& a)
{
	c_Connection->write(a);
}

void GunCtrl::NetworkRead()
{
	if (!c_Connection->isValid()) return;

	while (c_Connection->bytesAvailable() > 0)
	{
		char data[100];
		c_Connection->readLine(data, 100);
		QString command(data);
        NetCommand(command.split(" ", QString::SkipEmptyParts));
	}

}

void GunCtrl::NetCommand(QStringList cmd)
{
	qDebug() << cmd;

	QString commandName = cmd.at(0).trimmed();
	if (commandName == "btn")
			{ 
        if (cmd.at(1).trimmed().toInt() == 1)
            if (PlayFlag == 0)
                    Run();
			}
	else if (commandName == "close")
	{
		run->write("0");
        //  Do we need to clean other files?
        run->close();
        period_ns->close();
        duty_ns->close();
        request->write("0");
        request->close();
        //  Delete after closing
        delete run;
        delete duty_ns;
        delete period_ns;
        delete request;
	}
	else
		{
			qDebug() << "Unknown command" ;
		}

}
void GunCtrl::StopShot()
{
    shot->setPower(0);
    PlayFlag = 0;
}

void GunCtrl::StartShot()
{
    shot->setPower(100);
    QTimer::singleShot(3000, this, SLOT(StopShot()));
}

void GunCtrl::StopReload()
{

}

void GunCtrl::StartReload()
{
   reload->setPower(100);
}


void GunCtrl::Startlift()
{
	SetServo(1600000);

	qDebug() << "Startlift";
    lift->setPower(-60);
    
}

void GunCtrl::Stoplift()
{
	qDebug() << "Stoplift";
    lift->setPower(0);
    reload->setPower(0);
    SetServo(1800000);
    QTimer::singleShot(8000, this, SLOT(StartShot()));

}

void GunCtrl::SetServo(int k)
{
    duty_ns->write(QString::number(k).toStdString().data());
    run->write("1");
    duty_ns->flush();
    run->flush();
}

void GunCtrl::Run()
{
    PlayFlag = 1;
    qDebug() << "Run";
    Startlift();
    StartReload();
    QTimer::singleShot(8000, this, SLOT(Stoplift()));
}


