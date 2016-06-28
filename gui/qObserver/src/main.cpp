#include "qobserver.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	qObserver w;
	w.show();

	return a.exec();
}
