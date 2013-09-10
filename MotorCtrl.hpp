#pragma once

//#include <QFile>
//#include <QSettings>
#include "I2cConnection.hpp"


class Motor : public QObject
{
	Q_OBJECT
public:
	Motor(char jack,int period, I2cConnection* i2cCon);

	int setPower(int _power); 
	int setPeriod(int _period);
	char getPowerRegister(char jack);
	char getPeriodRegister(char jack);


public slots:
	int emergencyStop();

signals:
	int toLog(QString msg);

protected:
	unsigned char m_jack; 
	int m_power;
	I2cConnection* m_i2cCon;
};