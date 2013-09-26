#pragma once

#include <QObject>
#include <linux/types.h>

class I2cConnection : public QObject
{
	Q_OBJECT
public:
	I2cConnection(QString DevPath, int DevId);

	int OpenConnection();
	int SendData(char *data, char size);
	uint16_t ReceiveData(uint8_t reg);
	int CloseConnection();

signals:
	int toLog(QString msg);

protected:
	char i_BusFd;
	int i_DevId;
	QString i_DevPath;
};

#define I2C_SMBUS_BLOCK_MAX 32
#define I2C_SMBUS_READ 1
#define I2C_SMBUS_WRITE     0
#define I2C_SMBUS_WORD_DATA     3
#define I2C_SMBUS_BYTE_DATA     2

union i2c_smbus_data 
{
        __u8 byte;
        __u16 word;
        __u8 block[I2C_SMBUS_BLOCK_MAX + 2]; /* block[0] is used for length */
                                                    /* and one more for PEC */
};


