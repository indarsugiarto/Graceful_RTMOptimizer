#ifndef QOBSERVER_H
#define QOBSERVER_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>

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
};

#endif // QOBSERVER_H
