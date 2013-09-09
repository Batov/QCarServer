#include "MotorCtrl.hpp"

Motor::Motor(char jack):
m_power(0),
m_jack(jack)
{
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