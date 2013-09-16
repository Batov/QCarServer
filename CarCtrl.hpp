#pragma once

#include <QTcpServer>
#include <QSocketNotifier>
#include <QSettings>
#include <QtGui/QKeyEvent>
#include <QWidget>

#include "SideCtrl.hpp"
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

protected slots:
	void Connection();
	void Disconnected();
	void NetworkRead();
	void initSides();
	void initSettings();

signals:

protected:

	//virtual
	void  keyPressEvent(QKeyEvent *event);
	char c_StopFlag;

	QMap<QString, Side*> c_sides;

	QSettings* c_settings;
	QSettings* c_defaultSettings;

	QTcpServer  c_Server;
	QTcpSocket* c_Connection;

	I2cConnection* c_i2cCon;
};