#ifndef QOBSERVER_H
#define QOBSERVER_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QCloseEvent>

#include "qtester.h"

#define N_STATES	16

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

protected:
	void closeEvent(QCloseEvent *e);
};

#endif // QOBSERVER_H
