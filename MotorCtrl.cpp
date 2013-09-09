#include "MotorCtrl.hpp"

Motor::Motor(char jack, int period, i2cConnection* i2cCon):
m_power(0),
m_jack(jack),
m_i2cCon(i2cCon)
{
	setPeriod(period)
	setPower(0);
}

void Motor::setPower(int _power)
{
}

void Motor::setPeriod(int _period)
{
}

void Motor::emergencyStop()
{
	setPower(0);
}