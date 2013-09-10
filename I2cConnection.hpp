#pragma once

#include <QObject>


class I2cConnection : public QObject
{
	Q_OBJECT
public:
	I2cConnection(QString DevPath, int DevId);

	int OpenConnection();
	int SendData(char adress, int data);
	int CloseConnection();

signals:
	int toLog(QString msg);

protected:
	char i_BusFd;
	int i_DevId;
	QString i_DevPath;
};