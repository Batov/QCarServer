#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/i2c-dev.h>

#include <QtGui>

#include "I2cConnection.hpp"

static inline __s32 i2c_smbus_access(int file, char read_write, __u8 command, 
                                      int size, union i2c_smbus_data *data)

{        
        struct i2c_smbus_ioctl_data args;
 
        args.read_write = read_write;
        args.command = command;
        args.size = size;
        args.data = data;
        return ioctl(file,I2C_SMBUS,&args);
}

static inline __s32 i2c_smbus_read_word_data(int file, __u8 command)
 {
        union i2c_smbus_data data;
        if (i2c_smbus_access(file,I2C_SMBUS_READ,command,
                             I2C_SMBUS_WORD_DATA,&data))
               return -1;
        else
               return 0x0FFFF & data.word;
 }

 static inline __s32 i2c_smbus_write_word_data(int file, __u8 command, 
                                               __u16 value)
 {
        union i2c_smbus_data data;
        data.word = value;
        return i2c_smbus_access(file,I2C_SMBUS_WRITE,command,
                                I2C_SMBUS_WORD_DATA, &data);
}

static inline __s32 i2c_smbus_write_byte_data(int file, __u8 command, 
                                               __u8 value)
 {
        union i2c_smbus_data data;
        data.byte = value;
        return i2c_smbus_access(file,I2C_SMBUS_WRITE,command,
                                I2C_SMBUS_BYTE_DATA, &data);
 }


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
  if (ioctl(i_BusFd, I2C_SLAVE, i_DevId) != 0)
  {
    res = errno;
    fprintf(stdout, "ioctl(%d, I2C_SLAVE, %d) failed: %d\n", i_BusFd, i_DevId, res);
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
  if (size == 2) 
  {
    i2c_smbus_write_byte_data(i_BusFd, data[0], data[1]);
  }
  else
  {
    i2c_smbus_write_word_data(i_BusFd, data[0], data[1] | (data[2] << 8)); 
  }

return 0;
}

uint16_t I2cConnection::ReceiveData(uint8_t reg)
{  
  printf("register = 0x%x\n",reg);
  int rest = i2c_smbus_read_word_data(i_BusFd,reg);
  printf("0x%x\n",rest);
  return rest;
}
