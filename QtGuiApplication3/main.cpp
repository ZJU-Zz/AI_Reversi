#include "QtGuiApplication3.h"
#include <QtWidgets/QApplication>
#include "ReversiBoard.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QtGuiApplication3 w;
	QObject::connect(&w, &QtGuiApplication3::viewChanged,&w , &QtGuiApplication3::showView);
	QObject::connect(&w, &QtGuiApplication3::gameOver, &w, &QtGuiApplication3::endMessage);
	QObject::connect(&w, &QtGuiApplication3::timingStart, &w, &QtGuiApplication3::startTiming);
	QObject::connect(&w, &QtGuiApplication3::timingEnd, &w, &QtGuiApplication3::endTiming);
	w.show();
	return a.exec();
}
