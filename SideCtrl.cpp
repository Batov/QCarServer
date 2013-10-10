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

#include "SideCtrl.hpp"

Side::Side(char central,char edge,int period, I2cConnection* i2cCon):
s_power(0)
{
	s_motor_for_central_wheel = new Motor(central,period,i2cCon);
	s_motor_for_edge_wheels = new Motor(edge,period,i2cCon);
}

int Side::setPower(int _power)
{
	s_motor_for_central_wheel->setPower(_power);
	s_motor_for_edge_wheels->setPower(_power);
	return 0;
} 
