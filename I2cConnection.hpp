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

#include <QObject>

class I2cConnection : public QObject
{
	Q_OBJECT
public:
	I2cConnection(QString DevPath, int DevId);

	int OpenConnection();
	int SendData(char* data, char size);
	int CloseConnection();

signals:
	int toLog(QString msg);

protected:
	char i_BusFd;
	int i_DevId;
	QString i_DevPath;
};