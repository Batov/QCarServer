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

#pragma once

#include "I2cConnection.hpp"


class Motor : public QObject
{
	Q_OBJECT
public:
	Motor(char jack,int period, I2cConnection* i2cCon);

	int setPower(int _power); 
	int setPeriod(int _period);
	char getPowerRegister(char jack);
	char getPeriodRegister(char jack);


public slots:

signals:
	int toLog(QString msg);

protected:
	unsigned char m_jack; 
	int m_power;
	I2cConnection* m_i2cCon;
	const char min_Period = 200;
};