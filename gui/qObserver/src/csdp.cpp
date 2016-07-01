#include "csdp.h"
#include <QDebug>

cSDP::cSDP(QObject *parent) : QObject(parent),
	dsQReport(baQReport),
	dsMReport(baMReport),
	QrowCntr(0),
	_Ready(false)
{
	bool bQReport, bMReport;
	sdpQReport = new QUdpSocket(this);
	bQReport = sdpQReport->bind(SDP_UDP_PORT_REPORT);
	connect(sdpQReport, SIGNAL(readyRead()), this, SLOT(readQReport()));
	dsQReport.setVersion(QDataStream::Qt_4_8);
	dsQReport.setByteOrder(QDataStream::LittleEndian);
	if(bQReport)
		qDebug() << QString("Opening port-%1 for report OK!").arg(SDP_UDP_PORT_REPORT);
	else
		qDebug() << QString("Opening port-%1 for report fail!").arg(SDP_UDP_PORT_REPORT);

	sdpMReport = new QUdpSocket(this);
	bMReport = sdpMReport->bind(SDP_UDP_PORT_GENERIC);
	connect(sdpMReport, SIGNAL(readyRead()), this, SLOT(readMReport()));
	dsMReport.setVersion(QDataStream::Qt_4_8);
	dsMReport.setByteOrder(QDataStream::LittleEndian);
	if(bMReport)
		qDebug() << QString("Opening port-%1 for generic OK!").arg(SDP_UDP_PORT_GENERIC);
	else
		qDebug() << QString("Opening port-%1 for generic fail!").arg(SDP_UDP_PORT_GENERIC);

	_Ready = bQReport && bMReport;

	qP = (qParam_t *)malloc(sizeof(qParam_t));
}

cSDP::~cSDP()
{
	delete sdpQReport;
	delete sdpMReport;
	free(qP);
}

/* Note: How SDP data is encapsulated in QByteArray ba:
 * pad = ba[0:1]
 * flag = ba[2]
 * tag = ba[3]
 * dest_port = ba[4]
 * srce_port = ba[5]
 * dest_addr = ba[6:7]
 * srce_addr = ba[8:9]
 * cmd_rc = ba[10:11]
 * seq = ba[12:13]
 * arg1 = ba[14:17]
 * arg2 = ba[18:21]
 * arg3 = ba[22:25]
 * data = ba[25:-]
 * */

// Q-matrix will be sent row-by-row. Each Q-value is quint32.
// It uses cmd_rc to indicate the row-index, and the rest for Q-data.
void cSDP::readQReport()
{
	baQReport.resize(sdpQReport->pendingDatagramSize());
	sdpQReport->readDatagram(baQReport.data(), baQReport.size());
	/*
	QString str(baQReport.toHex());
	str.prepend("QReport = ");
	qDebug() << str;
	return;
	*/

	baQReport.remove(0, 12);	// remove header and cmd_rc
	quint16 row = getUshort(baQReport);	// read from seq
	//qDebug() << QString("Row-%1").arg(row);
	//return;

	baQReport.remove(0, 2+3*sizeof(quint32));		// remove seq and arg*

	if(baQReport.size() != N_STATES*sizeof(quint32))
		qDebug() << "Packet drop detected";

	quint32 qVal;
	for(int i=0; i<N_STATES; i++) {
		qVal = getUint(baQReport, i*sizeof(quint32));
		Q[QrowCntr*N_STATES + i] = qVal;
	}
	/*
	uchar * ptrQ = (uchar *)Q;
	ptrQ += row*N_STATES*sizeof(quint32);
	memcpy(ptrQ, baQReport.data(), baQReport.size());
	*/
	QrowCntr++;
	if(QrowCntr==N_STATES) {
		for(int i=0; i<N_STATES; i++) {
			QString str;
			for(int j=0; j<N_STATES; j++) {
				str.append(QString("%1 ").arg(Q[i*N_STATES + j]));
			}
			str.prepend(QString("Row-%1: ").arg(i));
			// qDebug() << str;
		}

		emit qMatrixReady((quint32 *)Q);
		QrowCntr = 0;
	}
}

// This is how genericMsg is used for reporting measurement:
/* format:
 *		cmd_rc = CPU performance
 *		seq = clock freqency
 *		arg1 = tempVal[0]
 *		arg2 = tempVal[2]
 *		arg3 = currentTempReal
 *		data[0:3] = reward value
 *		data[4:7] = average cpu utilization
 *		data[8:11] = phycpu_0_idle_cntr
 *		data[12:15] = phycpu_1_idle_cntr
 *		... etc
 * */
void cSDP::readMReport()
{
	baMReport.resize(sdpMReport->pendingDatagramSize());
	sdpMReport->readDatagram(baMReport.data(), baMReport.size());

	// check the size: it should be 2+8+16+8+18*4 = 34+72 = 106

	if(baMReport.size() != 106) {
		qDebug() << "Size mismatch";
		return;
	}

	/*
	QString str(baMReport.toHex());
	str.prepend("MReport = ");
	qDebug() << str;
	return;
	*/

	// remove header
	baMReport.remove(0,10);

	// read cmd_rc == CPU performance
	qP->CPUperf = getUshort(baMReport);
	//qDebug() << QString("CPU performance = %1").arg(qP->CPUperf);

	// read seq == clock frequency
	qP->freq = getUshort(baMReport, 2);
	//qDebug() << QString("Freq = %1").arg(qP->freq);

	// read arg1 == tempVal[0]
	qP->tempVal0 = getUint(baMReport, 4);
	//qDebug() << QString("tempVal0 = %1").arg(qP->tempVal0);

	// read arg2 == tempVal[2]
	qP->tempVal2 = getUint(baMReport, 8);
	//qDebug() << QString("tempVal2 = %1").arg(qP->tempVal2);

	// read arg3 == currentTempReal
	qP->tempReal = getREAL(baMReport, 12);
	//qDebug() << QString("tempReal = %1").arg(qP->tempReal);

	// read data[0:3] == reward value
	qP->rewardVal = getUint(baMReport, 16);
	//qDebug() << QString("Reward value = %1").arg(qP->rewardVal);

	// read data[4:7] == average cpu utilization
	qP->avgCPU = getUint(baMReport, 20);
	//qDebug() << QString("average CPU utilization = %1").arg(qP->avgCPU);

	// read data[8:-] == phycpu_idle_cntr
	QString str("CPU-idle: ");
	for(int i=0; i<18; i++) {
		qP->cpuIdle[i] = getUint(baMReport, 24+i*4);
		str.append(QString("%1 ").arg(qP->cpuIdle[i]));
	}
	qDebug() << str;
	emit qParamReady(qP);
}

quint16 cSDP::getUshort(QByteArray ba, uchar startIdx)
{
	quint16 result;
	QByteArray baUshort(2, '\0');
	baUshort[0] = ba[startIdx];
	baUshort[1] = ba[startIdx + 1];

	QDataStream dsUshort(baUshort);
	dsUshort.setVersion(QDataStream::Qt_4_8);
	dsUshort.setByteOrder(QDataStream::LittleEndian);
	dsUshort >> result;

	/*
	QString str(baUshort.toHex());
	str.prepend("In getUshort: ");
	qDebug() << str;
	*/

	return result;
}

quint32 cSDP::getUint(QByteArray ba, uchar startIdx)
{
	quint32 result;
	QByteArray baUint(4, '\0');
	for(int i=0; i<4; i++)
		baUint[i] = ba[startIdx + i];

	QDataStream dsUint(baUint);
	dsUint.setVersion(QDataStream::Qt_4_8);
	dsUint.setByteOrder(QDataStream::LittleEndian);
	dsUint >> result;

	/*
	QString str(baUint.toHex());
	str.prepend("In getUint: ");
	qDebug() << str;
	*/

	return result;
}

float cSDP::getREAL(QByteArray ba, uchar startIdx)
{
	float result;

	QByteArray baUint(4, '\0');
	for(int i=0; i<4; i++)
		baUint[i] = ba[startIdx + i];
	memcpy((void *)&result, baUint.data(), baUint.size());

	/*
	QString str(baUint.toHex());
	str.prepend("In getREAL: ");
	qDebug() << str;
	*/

	return result;
}
