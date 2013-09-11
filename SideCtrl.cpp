#include "SideCtrl.hpp"

Side::Side(char central,char edge,int period, I2cConnection* i2cCon):
s_power(0)
{
	s_motor_for_central_wheel = new Motor(central,period,i2cCon);
	s_motor_for_edge_wheels = new Motor(edge,period,i2cCon);
	setPower(0);
}

int Side::setPower(int _power)
{
	s_motor_for_central_wheel->setPower(_power);
	s_motor_for_edge_wheels->setPower(_power);
	return 0;
}

int Side::emergencyStop()
{
	setPower(0);
	return 0;
}