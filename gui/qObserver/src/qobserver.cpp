#include "qobserver.h"

#include <QDebug>

qObserver::qObserver(QWidget *parent) :
	QWidget(parent)
{
	setupUi();
}

qObserver::~qObserver()
{
	delete Q;
}

void qObserver::setupUi()
{
	int i,j,x, y, w = 40, h = 30;
	int offX = 10, offY = 10, spc = 2;
	Q = new QVector<QLineEdit *>();

	for(i=0; i<N_STATES; i++) {
		for(j=0; j<N_STATES; j++) {
			x = offX + i*w + spc;
			y = offY + j*h;
			QLineEdit *q = new QLineEdit(QString("0"),this);
			q->setGeometry(x,y,w,h);
			q->setAlignment(Qt::AlignCenter);
			Q->append(q);
		}
	}

	// then add the bottom field
	y = 515;
	lTemp.setParent(this);
	lTemp.setAlignment(Qt::AlignBottom | Qt::AlignRight);
	lTemp.setText("Temp");
	lTemp.setGeometry(20, y, 40, 20);
	temp.setParent(this);
	temp.setGeometry(65,510,50,30);
	temp.setText("0");
	lFreq.setParent(this);
	lFreq.setAlignment(Qt::AlignBottom | Qt::AlignRight);
	lFreq.setText("Freq");
	lFreq.setGeometry(135,y,40,20);
	freq.setParent(this);
	freq.setGeometry(180,510,50,30);
	freq.setText("0");
	lCPU.setParent(this);
	lCPU.setAlignment(Qt::AlignBottom | Qt::AlignRight);
	lCPU.setText("CPU");
	lCPU.setGeometry(250,y,40,20);
	util.setParent(this);
	util.setGeometry(290,510,50,30);
	util.setText("0");
	lPerf.setParent(this);
	lPerf.setAlignment(Qt::AlignBottom | Qt::AlignRight);
	lPerf.setText("Perf");
	lPerf.setGeometry(355,y,40,20);
	perf.setParent(this);
	perf.setGeometry(400,510,50,30);
	perf.setText("0");
	lRewd.setParent(this);
	lRewd.setAlignment(Qt::AlignBottom | Qt::AlignRight);
	lRewd.setText("Reward");
	lRewd.setGeometry(540,513,50,20);
	rewd.setParent(this);
	rewd.setGeometry(590,510,50,30);
	rewd.setText("0");
	this->setMinimumHeight(550);
	this->setMaximumHeight(550);
	this->setMinimumWidth(660);
	this->setMaximumWidth(660);
	//qDebug() << QString("%1, %2").arg(this->width()).arg(this->height());
}

