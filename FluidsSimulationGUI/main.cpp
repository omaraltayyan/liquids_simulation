#include "MainWindow.h"
#include <QtWidgets/QApplication>
#include "UnitTests\UnitTestGrid.h"

int main(int argc, char *argv[])
{
	auto grid = UnitTestGrid();
	grid.run();
	/*
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();*/
}
