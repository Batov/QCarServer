#pragma once

//#include <QFile>
//#include <QSettings>
#include "I2cConnection.hpp"


class Motor : public QObject
{
	Q_OBJECT
public:
	Motor(char jack,int period, I2cConnection* i2cCon);

	void setPower(int _power); 
	void setPeriod(int _period);

public slots:
	void emergencyStop();

signals:
	void toLog(QString msg);

protected:
	unsigned char m_jack; 
	int m_power;
	I2cConnection* m_i2cCon;
};