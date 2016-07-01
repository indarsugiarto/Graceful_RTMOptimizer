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
	bool isReady() {return _Ready;}
	quint32 Q[N_STATES*N_STATES];

signals:
	void qParamReady(qParam_t *p);
	void qMatrixReady(quint32 *q);

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

	int QrowCntr;
	qParam_t *qP;

	bool _Ready;

	// helper
	quint16 getUshort(QByteArray ba, uchar startIdx = 0);
	quint32 getUint(QByteArray ba, uchar startIdx = 0);
	float getREAL(QByteArray ba, uchar startIdx = 0);
};

#endif // CSDP_H
