#pragma once

#include <QFile>
#include <QSettings>

class CarMotor : public QObject
{
	Q_OBJECT
public:
	CarMotor(const QString& name = QString(),char jack);
	~CarMotor();

	void setPower(int _power); 

public slots:
	void emergencyStop();

signals:
	void toLog(QString msg);

protected:
	QString m_name;
	unsigned char m_jack; 
	int m_power;
};