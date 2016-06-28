#include "csdp.h"

cSDP::cSDP(QObject *parent) : QObject(parent)
{
	sdpReport = new QUdpSocket(this);
}

cSDP::~cSDP()
{
	delete sdpReport;
}

void cSDP::readReport()
{

}
