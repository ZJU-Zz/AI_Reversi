#include "QtGuiApplication3.h"
#include <QtWidgets/QApplication>
#include "ReversiBoard.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QtGuiApplication3 w;
	QObject::connect(&w, &QtGuiApplication3::viewChanged,&w , &QtGuiApplication3::showView);
	w.show();
	return a.exec();
}
