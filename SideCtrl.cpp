#include "SideCtrl.hpp"

Side::Side(char central,char edge,int period, i2cConnection* i2cCon):
s_power(0);
s_motor_for_central_wheel = new Motor(central,period,i2cCon);
s_motor_for_edge_wheels = new Motor(edge,period.i2cCon);
{
	setPower(0);
}

void Side::setPower(int _power)
{
}

void Side::setPeriod(int _period)
{
}

void Side::emergencyStop()
{
	setPower(0);
}