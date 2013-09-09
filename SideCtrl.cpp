#include "SideCtrl.hpp"

Side::Side(char central,char edge):
s_power(0);
s_motor_for_central_wheel = new Motor(central);
s_motor_for_edge_wheels = new Motor(edge);
{
	setPower(0);
}

void Side::setPower(int _power)
{
}

void Side::emergencyStop()
{
	setPower(0);
}