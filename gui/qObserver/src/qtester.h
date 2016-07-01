#ifndef QTESTER_H
#define QTESTER_H

#include <QWidget>
#include <QUdpSocket>

#include "spinq.h"

namespace Ui {
class QTester;
}

class QTester : public QWidget
{
	Q_OBJECT

public:
	explicit QTester(QWidget *parent = 0);
	~QTester();
public slots:
	void pbSetClicked();
	void pbGoClicked();
	void pbPLLClicked();
	void populateChips(int spinn = 0);
	void sendCmd(cmd_hdr_t cmd);

private:
	// variables
	Ui::QTester *ui;
	QUdpSocket sender;
	// functions
	void sendSDP(sdp_hdr_t h, QByteArray s = QByteArray(), QByteArray d = QByteArray());
	QByteArray scp(cmd_hdr_t cmd);
	QByteArray hdr(sdp_hdr_t h);
};

#endif // QTESTER_H
