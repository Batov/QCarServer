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
	if (_power > 100) _power = 100;
	if (_power < -100) _power = -100;

	char data[2];
	data[0] = getPowerRegister(m_jack);
	data[1] = (_power)&0xff;
	printf("Set Power  : Motor jack = %d Power=%x \n",m_jack,data[1]);
	m_i2cCon->SendData(data,sizeof(data));
	return 0;
}

int Motor::setPeriod(int _period)
{
	if (_period < min_Period) _period = min_Period;

	char data[3];
	data[0] = getPeriodRegister(m_jack);
	data[1] = (_period)&0xff;
	data[2] = (_period >> 8)&0xff;
	printf("Set Period : Motor jack = %d Period=%x%x \n",m_jack,data[2],data[1]);
	//TODO: check it
	m_i2cCon->SendData(data,sizeof(data));
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
		case 3: return 0x17;
		case 4: return 0x16;
	}
	return 0;
}

char Motor::getPeriodRegister(char jack)
{
	switch (jack)
	{
		case 1: return 0x10;
		case 2: return 0x11;
		case 3: return 0x13;
		case 4: return 0x12;
	}
	return 0;
}