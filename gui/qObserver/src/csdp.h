#ifndef CSDP_H
#define CSDP_H

#include <QObject>
#include <QUdpSocket>

class cSDP : public QObject
{
	Q_OBJECT
public:
	explicit cSDP(QObject *parent = 0);
	~cSDP();

signals:

public slots:
	void readReport();

private:
	QUdpSocket *sdpReport;
};

#endif // CSDP_H
