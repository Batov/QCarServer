#pragma once

//#include <QFile>
//#include <QSettings>

#include "MotorCtrl.hpp"
#include "I2cConnection.hpp"

class Side : public QObject
{
	Q_OBJECT
public:
	Side(char central,char edge, int period, I2cConnection* i2cCon);

	int setPower(int _power);

public slots:
	int emergencyStop();

signals:
	int toLog(QString msg);

protected:
	int s_power;
	Motor* s_motor_for_central_wheel;
	Motor* s_motor_for_edge_wheels;
};