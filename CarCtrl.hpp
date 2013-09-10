#pragma once

#include <QTcpServer>
#include <QSocketNotifier>
#include <QSettings>
#include <QVector3D>

#include "SideCtrl.hpp"


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
	QSettings* getSettings() { return c_settings; } 

public slots:
	void Responce(const QByteArray& a);
	void emergencyStop();

protected slots:
	void Connection();
	void Disconnected();
	void NetworkRead();
	void initSides();
	void initSettings();

signals:

protected:
	//void timerEvent(QTimerEvent *event);
	QMap<QString, Side*> c_sides;

	QSettings* c_settings;
	QSettings* c_defaultSettings;

	QTcpServer  c_Server;
	QTcpSocket* c_Connection;
	int c_timerId;
};