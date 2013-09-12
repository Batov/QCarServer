#pragma once

#include <QTcpServer>
#include <QSocketNotifier>
#include <QSettings>
#include <QVector3D>

#include "SideCtrl.hpp"
#include "I2cConnection.hpp"


#if QT_VERSION >= 0x050000  // wtf?
#include <QApplication>
#else
#include <QtGui/QApplication>
#endif

class CarCtrl : public QObject
{
	Q_OBJECT
public:
	CarCtrl();
	~CarCtrl();
	QSettings* getSettings() { return c_settings; } 

public slots:
	void Responce(const QByteArray& a);
	void emergencyStop();
	void resumeMoving();
	void Stop();

protected slots:
	void Connection();
	void Disconnected();
	void NetworkRead();
	void initSides();
	void initSettings();

signals:

protected:
	char c_StopFlag;
	QMap<QString, Side*> c_sides;

	QSettings* c_settings;
	QSettings* c_defaultSettings;

	QTcpServer  c_Server;
	QTcpSocket* c_Connection;

	I2cConnection* c_i2cCon;
};