#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/i2c-dev.h>

#include "I2cConnection.hpp"

I2cConnection::I2cConnection(QString DevPath, int DevId):
i_DevPath(DevPath),
i_DevId(DevId),
i_BusFd(0)
{
	OpenConnection();
}

int I2cConnection::OpenConnection()
{
	int res;
	i_BusFd= open(i_DevPath.toAscii().data(),O_RDWR);
	if (i_BusFd< 0)
  		{
    		res = errno;
    		fprintf(stdout, "open(%s) failed: %d\n", i_DevPath.toAscii().data(), res);
    		i_BusFd = -1;
    		return res;
  		}
  	return 0;
}

int I2cConnection::CloseConnection()
{
	int res;
	if (close(i_BusFd) != 0)
  		{
    		res = errno;
    		fprintf(stdout, "close() failed: %d\n", res);
    		return res;
  		}
  	i_BusFd = -1;
  	return 0;
}

int I2cConnection::SendData(char* data, char size)
{
  int res;
  if (ioctl(i_BusFd, I2C_SLAVE, i_DevId) != 0)
  {
    res = errno;
    fprintf(stdout, "ioctl(%d, I2C_SLAVE, %d) failed: %d\n", i_BusFd, i_DevId, res);
    return res;
  }
   unsigned char cmd[size];
  for (int i = 0; i < size; ++i)
  {
  	cmd[i] = data[i];
  }

  if ((res = write(i_BusFd, &cmd, sizeof(cmd)) != sizeof(cmd)))
  {
    if (res >= 0)
      res = E2BIG;
    else
      res = errno;
    fprintf(stdout, "write(%d) failed: %d\n", i_BusFd, res);
    return res;
  }

return 0;
}

uint16_t I2cConnection::ReceiveData(uint8_t reg)
{
  // int res;
  // if (ioctl(i_BusFd, I2C_SLAVE, i_DevId) != 0)
  // {
  //   res = errno;
  //   fprintf(stdout, "ioctl(%d, I2C_SLAVE, %d) failed: %d\n", i_BusFd, i_DevId, res);
  //   return res;
  // }

  uint8_t data[3];
  data[0]= reg;

  if (write(i_BusFd, data, 1) != 1) // may be use &
  { 
    fprintf(stdout, "write register failed \n");
    return 1;
  }
  
  if (read(i_BusFd, data, 2) != 2) 
  {
    fprintf(stdout, "read failed \n");
    return 1;
  }
  
  return data[0] | (data[1] << 8);
}

