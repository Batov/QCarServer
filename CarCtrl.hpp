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

#include "MotorCtrl.hpp"
#include "I2cConnection.hpp"


#if QT_VERSION >= 0x050000  // wtf?
#include <QApplication>
#else
#include <QtGui/QApplication>
#endif

class CarCtrl : public QWidget
{
	Q_OBJECT
public:
	CarCtrl();
	~CarCtrl();

	QSettings* getSettings() { return c_settings; } 

public slots:
	void Responce(const QByteArray& a);
	void EmergencyStop();
	void ResumeMoving();
	void Stop();
	void Run(QStringList);

protected slots:
	void Connection();
	void Disconnected();
	void NetworkRead();
	void initMotors();
	void initSettings();

signals:

protected:

	//virtual
	void  keyPressEvent(QKeyEvent *event);
	char c_StopFlag;

	QMap<QString, Motor*> c_motors;

	QSettings* c_settings;
	QSettings* c_defaultSettings;

	QTcpServer  c_Server;
	QTcpSocket* c_Connection;

	I2cConnection* c_i2cCon;
};