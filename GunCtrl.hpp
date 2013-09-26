#pragma once

#include <QTcpServer>
#include <QSocketNotifier>
#include <QSettings>
#include <QtGui/QKeyEvent>
#include <QWidget>
#include <QIODevice>

#include "MotorCtrl.hpp"
#include "I2cConnection.hpp"


#if QT_VERSION >= 0x050000  // wtf?
#include <QApplication>
#else
#include <QtGui/QApplication>
#endif

class GunCtrl : public QWidget
{
	Q_OBJECT
public:
	GunCtrl();
	~GunCtrl();

	QSettings* getSettings() { return c_settings; } 

public slots:
	void Responce(const QByteArray& a);
	void StopShot();
	void Shot();
	void Stop();
	void Run(QStringList);

protected slots:
	void Connection();
	void Disconnected();
	void NetworkRead();
	void initMotor();
	void initSettings();
	void initServo();

signals:

protected:

	char ShotFlag;

	Motor *lift;
	Motor *shot;

	QFile * request;        //  1 - open others for write, 0 - close interface
    QFile * period_ns;      //  Signal frequency
    QFile * duty_ns;        //  Width of 1-signal in ns
    QFile * run;            //  Run signal
    QString * path;

	QSettings* c_settings;
	QSettings* c_defaultSettings;

	QTcpServer  c_Server;
	QTcpSocket* c_Connection;

	I2cConnection* c_i2cCon;
};