// Copyright 2013 Batov

//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at

//        http://www.apache.org/licenses/LICENSE-2.0

//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.

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
	m_i2cCon->SendData(data,sizeof(data));
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