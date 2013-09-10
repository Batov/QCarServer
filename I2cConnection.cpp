#include "I2cConnection.hpp"

I2cConnection::I2cConnection(QString DevPath, int DevId):
i_DevPath(DevPath),
i_DevId(DevId)
{
	OpenConnection();
}

void I2cConnection::OpenConnection()
{

}

void I2cConnection::CloseConnection()
{

}

void I2cConnection::SendData()
{

}
