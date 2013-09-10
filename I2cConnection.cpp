#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
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
    		fprintf(stderr, "open(%s) failed: %d\n", i_DevPath.toAscii().data(), res);
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
    		fprintf(stderr, "close() failed: %d\n", res);
    		return res;
  		}
  	i_BusFd = -1;
  	return 0;
}

int I2cConnection::SendData(char adress, int data)
{
  int res;
  if (ioctl(i_BusFd, I2C_SLAVE, i_DevId) != 0)
  {
    res = errno;
    fprintf(stderr, "ioctl(%d, I2C_SLAVE, %d) failed: %d\n", i_BusFd, i_DevId, res);
    return res;
  }

   unsigned char cmd[2];
   cmd[0] = adress&0xff;
   cmd[1] = data&0xff; //TODO fix it!

  if ((res = write(i_BusFd, &cmd, sizeof(cmd)) != sizeof(cmd)))
  {
    if (res >= 0)
      res = E2BIG;
    else
      res = errno;
    fprintf(stderr, "write(%d) failed: %d\n", i_BusFd, res);
    return res;
  }

return 0;
}
