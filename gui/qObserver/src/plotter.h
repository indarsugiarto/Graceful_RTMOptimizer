#ifndef PLOTTER_H
#define PLOTTER_H

#include <QWidget>
#include <qwt/qwt.h>
#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>
#include <QPaintEvent>
#include <QVector>

#define DEF_MAX_VALUE	100
#define DEF_MIN_VALUE	0
#define X_POINTS		100

namespace Ui {
class Plotter;
}

class Plotter : public QWidget
{
	Q_OBJECT

public:
	explicit Plotter(QString title, double minVal = DEF_MIN_VALUE, double maxVal = DEF_MAX_VALUE, QColor clr = Qt::blue, QWidget *parent = 0);
	~Plotter();
	void setYTitle(QString ttl) {qwtPlot->setAxisTitle(qwtPlot->yLeft, ttl);}

public slots:
	void appendData(double d);

private:
	Ui::Plotter *ui;
	QwtPlot *qwtPlot;
	QwtPlotCurve *qwtCurve;

	double _minVal;
	double _maxVal;
	QVector<double> _y;
	QVector<double> _x;
protected:
	void paintEvent(QPaintEvent *e);
};

#endif // PLOTTER_H
