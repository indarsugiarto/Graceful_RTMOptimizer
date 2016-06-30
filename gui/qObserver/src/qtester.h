#ifndef QTESTER_H
#define QTESTER_H

#include <QWidget>
#include <QUdpSocket>

namespace Ui {
class QTester;
}

#define DEF_SEND_PORT	17893		// tidak bisa diganti dengan yang lain

typedef struct sdp_hdr		// SDP header
{
  uchar flags;
  uchar tag;
  uchar dest_port;
  uchar srce_port;
  quint16 dest_addr;
  quint16 srce_addr;
} sdp_hdr_t;

typedef struct cmd_hdr		// Command header
{
  quint16 cmd_rc;
  quint16 seq;
  quint32 arg1;
  quint32 arg2;
  quint32 arg3;
} cmd_hdr_t;



class QTester : public QWidget
{
	Q_OBJECT

public:
	explicit QTester(QWidget *parent = 0);
	~QTester();
public slots:
	void pbSetClicked();
	void pbGoClicked();
	void populateChips(int spinn = 0);

private:
	// variables
	Ui::QTester *ui;
	QUdpSocket sender;
	static uchar X_CHIPS[48];       // must be static, otherwise it'll raise
	static uchar Y_CHIPS[48];       // is not a static data member of cSpiNNcomm
	// functions
	void sendSDP(sdp_hdr_t h, QByteArray s = QByteArray(), QByteArray d = QByteArray());
	QByteArray scp(cmd_hdr_t cmd);
	QByteArray hdr(sdp_hdr_t h);
};

#endif // QTESTER_H
