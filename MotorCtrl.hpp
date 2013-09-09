#pragma once

//#include <QFile>
//#include <QSettings>

class Motor : public QObject
{
	Q_OBJECT
public:
	Motor(char jack,int period, i2cConnection* i2cCon);

	void setPower(int _power); 
	void setPeriod(int _period);

public slots:
	void emergencyStop();

signals:
	void toLog(QString msg);

protected:
	unsigned char m_jack; 
	int m_power;
	i2cConnection* m_i2cCon;
};