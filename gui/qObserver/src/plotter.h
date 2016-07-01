#ifndef PLOTTER_H
#define PLOTTER_H

#include <QWidget>
#include <qwt/qwt.h>
#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>
#include <QPaintEvent>

namespace Ui {
class Plotter;
}

class Plotter : public QWidget
{
	Q_OBJECT

public:
	explicit Plotter(QString title, QWidget *parent = 0);
	~Plotter();

private:
	Ui::Plotter *ui;
	QwtPlot *qwtPlot;
protected:
	void paintEvent(QPaintEvent *e);
};

#endif // PLOTTER_H
