// Copyright 2013 Batov

//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at

//        http://www.apache.org/licenses/LICENSE-2.0

//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.

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
    void NetCommand(QStringList);
    void Startlift();
    void Stoplift();
    void SetServo(int k);
    void StartReload(int time);
    void StopReload();
    void StartShot();
    void StopShot();

protected slots:
	void Connection();
	void Disconnected();
	void NetworkRead();
	void initMotor();
	void initSettings();
    void initServo();

signals:

protected:

	int StageofPlay;
	int LastTimeofLoad;
    const int loadPosition = 1600000;

	Motor *lift;
	Motor *shot;
	Motor *reload;

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
