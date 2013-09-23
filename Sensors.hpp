#pragma once

#include "I2cConnection.hpp"

class Sensors : public QObject
{
	Q_OBJECT
public:
    Sensors(I2cConnection* i2cCon);
	int getValue(int number);
	unsigned char getRegister(int number);
public slots:

signals:
	int toLog(QString msg);

protected:
	I2cConnection* s_i2cCon;
	
};
