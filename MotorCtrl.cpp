#include "MotorCtrl.hpp"

Motor::Motor(char jack, int period, I2cConnection* i2cCon):
m_power(0),
m_jack(jack),
m_i2cCon(i2cCon)
{
	setPeriod(period);
	setPower(0);
}

int Motor::setPower(int _power)
{
	m_i2cCon->SendData(getPowerRegister(m_jack,_power);
	return 0;
}

int Motor::setPeriod(int _period)
{
	m_i2cCon->SendData(getPeriodRegister(m_jack,_period);
	return 0;
}

int Motor::emergencyStop()
{
	setPower(0);
	return 0;
}

char Motor::getPowerRegister(char jack)
{
	switch (jack)
	{
		case 1: return 0x14;
		case 2: return 0x15;
		case 3: return 0x16;
		case 4: return 0x17;
	}
	return 0;
}

char Motor::getPeriodRegister(char jack)
{
	switch (jack)
	{
		case 1: return 0x10;
		case 2: return 0x11;
		case 3: return 0x12;
		case 4: return 0x13;
	}
	return 0;
}