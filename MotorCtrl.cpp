#include "CarMotor.hpp"

CarMotor::CarMotor(const QString& name, char jack):
m_power(0),
m_name(name),
m_jack(jack)
{
	setPower(0);
}

void CarMotor::setPower(int _power)
{
}

void CarMotor::emergencyStop()
{
	setPower(0);
}