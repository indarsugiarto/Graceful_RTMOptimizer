#ifndef QOBSERVER_H
#define QOBSERVER_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QCloseEvent>
#include <QtGui>

#include "spinq.h"
#include "qtester.h"
#include "plotter.h"
#include "csdp.h"

namespace Ui {
class qObserver;
}

class qObserver : public QWidget
{
	Q_OBJECT

public:
	explicit qObserver(QWidget *parent = 0);
	~qObserver();

public slots:
	void readQParam(qParam_t *p);
	void readQMatrix(quint32 *Qmatrix);
	void pbSetQClicked();

private:
	void setupUi();
	QVector <QLineEdit *> *Q;
	QLineEdit temp, freq, util, perf, rewd;
	QLabel lTemp, lFreq, lCPU, lPerf, lRewd;
	QPushButton *pbSetQ;

	cSDP *dataSource;

	QTester qTesterWidget;
	Plotter *Tplot;			// temperature
	Plotter *Fplot;			// frequency
	Plotter *Rplot;			// reward

	quint32 Qval[N_STATES*N_STATES];
	qParam_t *qP;

	bool qIsRunning;

protected:
	void closeEvent(QCloseEvent *e);
};

#endif // QOBSERVER_H
