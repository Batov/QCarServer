#include "Sensors.hpp"

Sensors::Sensors(I2cConnection* i2cCon):
s_i2cCon(i2cCon)
{

}

int Sensors::getValue(int number)
{
	return s_i2cCon->ReceiveData(getRegister(number));
}

char Sensors::getRegister(int number)
{
	switch (number)
	{
		case 1: return 0x20;
		case 2: return 0x21;
		case 3: return 0x22;
		case 4: return 0x23;
		case 5: return 0x23;
		case 6: return 0x25;
		case 7: return 0x26;
		case 8: return 0x27;
	}
}