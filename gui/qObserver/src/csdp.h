#ifndef CSDP_H
#define CSDP_H

#include <QObject>
#include <QUdpSocket>
#include <QDataStream>

#include "spinq.h"

class cSDP : public QObject
{
	Q_OBJECT
public:
	explicit cSDP(QObject *parent = 0);
	~cSDP();

signals:

public slots:
	void readQReport();
	void readMReport();

private:
	QUdpSocket *sdpQReport;	// Q-table
	QByteArray baQReport;
	QDataStream dsQReport;

	QUdpSocket *sdpMReport;	// Measurement
	QByteArray baMReport;
	QDataStream dsMReport;
};

#endif // CSDP_H
