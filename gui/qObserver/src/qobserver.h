#ifndef QOBSERVER_H
#define QOBSERVER_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QCloseEvent>

#include "spinq.h"
#include "qtester.h"
#include "plotter.h"

namespace Ui {
class qObserver;
}

class qObserver : public QWidget
{
	Q_OBJECT

public:
	explicit qObserver(QWidget *parent = 0);
	~qObserver();

private:
	void setupUi();
	QVector <QLineEdit *> *Q;
	QLineEdit temp, freq, util, perf, rewd;
	QLabel lTemp, lFreq, lCPU, lPerf, lRewd;

	QTester qTesterWidget;
	Plotter *Tplot;
	Plotter *Fplot;

protected:
	void closeEvent(QCloseEvent *e);
};

#endif // QOBSERVER_H
