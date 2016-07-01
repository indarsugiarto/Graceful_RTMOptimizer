#include "plotter.h"
#include "ui_plotter.h"

Plotter::Plotter(QString title, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Plotter)
{
	ui->setupUi(this);
	qwtPlot = new QwtPlot(title, this);
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
