#pragma once

#include <QTcpServer>
#include <QSocketNotifier>
#include <QSettings>

#include "SideCtrl.hpp"
#include "i2cConnection.hpp"

#if QT_VERSION >= 0x050000  // wtf?
#include <QApplication>
#else
#include <QtGui/QApplication>
#endif

class Car : public QObject
{
	Q_OBJECT
public:
	CarCtrl();
	QSettings* getSettings() { return m_settings; } 

public slots:
	void qrealResponce(const QByteArray& a);
	void emergencyStop();

protected slots:
	void onQRealConnection();
	void onQRealDisconnected();
	void onQRealNetworkRead();
	void initMotors();
	void initSettings();

signals:

protected:
	void timerEvent(QTimerEvent *event);
	QMap<QString, Side*> m_sides;

	QSettings* m_settings;
	QSettings* m_defaultSettings;

	QTcpServer  m_qrealServer;
	QTcpSocket* m_qrealConnection;
	int m_timerId;
};