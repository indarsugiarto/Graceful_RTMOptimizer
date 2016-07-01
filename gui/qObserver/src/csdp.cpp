#include "csdp.h"

cSDP::cSDP(QObject *parent) : QObject(parent),
	dsQReport(baQReport),
	dsMReport(baMReport)
{
	sdpQReport = new QUdpSocket(this);
	sdpQReport->bind(SDP_UDP_PORT_REPORT);
	connect(sdpQReport, SIGNAL(readyRead()), this, SLOT(readQReport()));
	dsQReport.setVersion(QDataStream::Qt_4_8);
	dsQReport.setByteOrder(QDataStream::LittleEndian);

	sdpMReport = new QUdpSocket(this);
	sdpMReport->bind(SDP_UDP_PORT_GENERIC);
	connect(sdpMReport, SIGNAL(readyRead()), this, SLOT(readMReport()));
	dsMReport.setVersion(QDataStream::Qt_4_8);
	dsMReport.setByteOrder(QDataStream::LittleEndian);
}

cSDP::~cSDP()
{
	delete sdpQReport;
	delete sdpMReport;
}

void cSDP::readQReport()
{

}

void cSDP::readMReport()
{

}
