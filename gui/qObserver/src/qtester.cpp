#include "qtester.h"
#include "ui_qtester.h"
#include <QHostAddress>

QTester::QTester(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::QTester)
{
	ui->setupUi(this);
	QStringList sList, fList;
	for(int row=0; row<N_STATES; row++) {
		fList.append(QString("%1").arg(100+(row*10)));
		for(int col=0; col<N_STATES; col++)
			sList.append(QString("[%1,%2]").arg(row).arg(col));
	}
	// populate cbStates and cbFreq
	ui->cbStates->addItems(sList);
	ui->cbFreq->addItems(fList);

	// populate chip list
	populateChips(ui->cbSpin->currentIndex());
	connect(ui->cbSpin, SIGNAL(currentIndexChanged(int)), this, SLOT(populateChips(int)));
}

QTester::~QTester()
{
	delete ui;
}

void QTester::populateChips(int spinn)
{
	int i, j;
	j = spinn==0?4:48;
	for(i=0; i<j; i++)
		ui->cbChip->addItem(QString("<%1,%2>").arg(X_CHIPS[i]).arg(Y_CHIPS[i]));
}

// set SpiNNaker frequency
void QTester::pbSetClicked()
{

}

// go to specific state
void QTester::pbGoClicked()
{
	quint32 f = ui->cbFreq->currentText().toInt();
	sdp_hdr_t h;
	quint16 x = X_CHIPS[ui->cbChip->currentIndex()];
	quint16 y = Y_CHIPS[ui->cbChip->currentIndex()];
	h.flags = 0x07;
	h.tag = 0;
	h.srce_addr = 0;
	h.srce_port = 255;
	h.dest_addr = (x << 8) + y;
	h.dest_port = (SDP_CMD_PORT << 5) + ui->sbCore->value();
	cmd_hdr_t c;
	c.cmd_rc = SDP_CMD_SET_FREQ;
	c.seq = f;
	sendSDP(h, scp(c));
}

void QTester::sendSDP(sdp_hdr_t h, QByteArray s, QByteArray d)
{
	QByteArray ba = QByteArray(2, '\0');    // pad first
	ba.append(hdr(h));

	if(s.size()>0) ba.append(s);

	if(d.size()>0) ba.append(d);

	if(ui->cbSpin->currentIndex()==0)
		sender.writeDatagram(ba, QHostAddress("192.168.240.253"), DEF_SEND_PORT);
	else
		sender.writeDatagram(ba, QHostAddress("192.168.240.1"), DEF_SEND_PORT);
	//sender->writeDatagram(ba, QHostAddress::Any, DEF_SEND_PORT);	// not working with Any
}

QByteArray QTester::scp(cmd_hdr_t cmd)
{
	QByteArray ba;

	QDataStream stream(&ba, QIODevice::WriteOnly);
	stream.setVersion(QDataStream::Qt_4_8);
	stream.setByteOrder(QDataStream::LittleEndian);

	stream << cmd.cmd_rc << cmd.seq << cmd.arg1 << cmd.arg2 << cmd.arg3;
	return ba;
}

QByteArray QTester::hdr(sdp_hdr_t h)
{
	QByteArray ba;

	QDataStream stream(&ba, QIODevice::WriteOnly);
	stream.setVersion(QDataStream::Qt_4_8);
	stream.setByteOrder(QDataStream::LittleEndian);

	stream << h.flags << h.tag << h.dest_port << h.srce_port << h.dest_addr << h.srce_addr;

	return ba;
}
