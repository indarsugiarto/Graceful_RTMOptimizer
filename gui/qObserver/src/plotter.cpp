#include "plotter.h"
#include "ui_plotter.h"

Plotter::Plotter(QString title, double minVal, double maxVal, QColor clr, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Plotter)
{
	_minVal = minVal;
	_maxVal = maxVal;

	ui->setupUi(this);
	qwtPlot = new QwtPlot(title, this);
	qwtCurve = new QwtPlotCurve();
	qwtCurve->setPen(clr, 2.0);

	// initialize value
	for(int i=0; i<X_POINTS; i++) {
		_x.append((double)i);
		_y.append(0.0);
	}
	qwtCurve->setSamples(_x, _y);
	qwtCurve->attach(qwtPlot);
	qwtPlot->replot();
}

Plotter::~Plotter()
{
	delete ui;
}

void Plotter::paintEvent(QPaintEvent *e)
{
	qwtPlot->resize(width(), height());
	e->accept();
}

void Plotter::appendData(double d)
{
	_y.prepend(d);
	_y.resize(X_POINTS);
	qwtCurve->setSamples(_x, _y);
	qwtPlot->replot();
}
