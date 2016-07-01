#include "qobserver.h"

#include <QDebug>
#include <QMessageBox>

qObserver::qObserver(QWidget *parent) :
	QWidget(parent),
	qP(NULL),
	qIsRunning(false)
{
	setupUi();

	qTesterWidget.show();
	qTesterWidget.setGeometry(0, 0, qTesterWidget.width(), qTesterWidget.height());
	Tplot = new Plotter("Chip Temperature", 0.0, 100.0, Qt::red);
	Tplot->setYTitle("deg");
	Tplot->show();
	Tplot->setGeometry(qTesterWidget.x()+qTesterWidget.width()+5, 0, Tplot->width(), Tplot->height());
	Fplot = new Plotter("Cores Clock Frequency", 10.0, 260.0, Qt::blue);
	Fplot->setYTitle("MHz");
	Fplot->show();
	Fplot->setGeometry(Tplot->x()+Tplot->width()+5,0,Fplot->width(), Fplot->height());
	Rplot = new Plotter("Reward Value", -1000.0, 1000.0, Qt::green);
	Rplot->setYTitle("Pts");
	Rplot->show();
	Rplot->setGeometry(Fplot->x()+Fplot->width()+5,0,Rplot->width(),Rplot->height());

	dataSource = new cSDP(this);

	if(!dataSource->isReady()) {
		QMessageBox::critical(this, "SDP Error", "Cannot open the port?");
		this->close();
	}
	else {
		qP = (qParam_t *)malloc(sizeof(qParam_t));
		connect(dataSource, SIGNAL(qMatrixReady(quint32 *)), this, SLOT(readQMatrix(quint32 *)));
		connect(dataSource, SIGNAL(qParamReady(qParam_t*)), this, SLOT(readQParam(qParam_t*)));
	}
}

qObserver::~qObserver()
{
	if(qP != NULL)
		free(qP);
}

void qObserver::closeEvent(QCloseEvent *e)
{
	if(qTesterWidget.isVisible())
		qTesterWidget.close();
	if(Tplot->isVisible())
		Tplot->close();
	if(Fplot->isVisible())
		Fplot->close();
	if(Rplot->isVisible())
		Rplot->close();
	e->accept();
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
			q->setReadOnly(true);
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
	lCPU.setGeometry(240,y,40,20);
	util.setParent(this);
	util.setGeometry(280,510,70,30);
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
	lRewd.setGeometry(460,513,50,20);
	rewd.setParent(this);
	rewd.setGeometry(510,510,50,30);
	rewd.setText("0");
	pbSetQ = new QPushButton("Run", this);
	pbSetQ->setGeometry(580,510,70,30);
	connect(pbSetQ, SIGNAL(pressed()), this, SLOT(pbSetQClicked()));
	this->setMinimumHeight(550);
	this->setMaximumHeight(550);
	this->setMinimumWidth(660);
	this->setMaximumWidth(660);
	//qDebug() << QString("%1, %2").arg(this->width()).arg(this->height());
}

void qObserver::pbSetQClicked()
{
	cmd_hdr_t c;
	if(qIsRunning==false) {
		pbSetQ->setText("Stop");
		qIsRunning = true;
		c.cmd_rc = SDP_CMD_RUNQ;
		qTesterWidget.sendCmd(c);
	}
	else {
		pbSetQ->setText("Run");
		qIsRunning = false;
		c.cmd_rc = SDP_CMD_STOPQ;
		qTesterWidget.sendCmd(c);
	}
}

void qObserver::readQMatrix(quint32 *Qmatrix)
{
	for(int i=0; i<N_STATES*N_STATES; i++) {
		Qval[i] = Qmatrix[i];
	}
	//qDebug() << dummy;

	// then display it

	for(int i=0; i<N_STATES; i++)
		for(int j=0; j<N_STATES; j++) {
			Q->at(j+i*N_STATES)->setText(QString("%1").arg(Qval[j+i*N_STATES]));
		}
}

void qObserver::readQParam(qParam_t *p)
{
	memcpy(qP, p, sizeof(qParam_t));
	temp.setText(QString("%1").arg(qP->tempReal, 0, 'f', 2));
	freq.setText(QString("%1").arg(qP->freq));
	util.setText(QString("%1").arg(qP->avgCPU));
	perf.setText(QString("%1").arg(qP->CPUperf));
	rewd.setText(QString("%1").arg(qP->rewardVal));

	Tplot->appendData((double)qP->tempReal);
	Fplot->appendData((double)qP->freq);
	Rplot->appendData((double)qP->rewardVal);
}
