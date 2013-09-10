#pragma once

#include <QObject>


class I2cConnection : public QObject
{
	Q_OBJECT
public:
	I2cConnection(QString DevPath, int DevId);

	void OpenConnection();
	void SendData();
	void CloseConnection();

signals:
	void toLog(QString msg);

protected:
	char i_DevFd;
	int i_DevId;
	QString i_DevPath;
};