#pragma once


class i2c_Connection : public QObject
{
	Q_OBJECT
public:
	i2cConnection(QString DevPath, int DevId);

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