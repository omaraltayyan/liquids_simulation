#include "MainWindow.h"
#include <QtWidgets/QApplication>
#include "UnitTests\UnitTestGrid.h"

int main(int argc, char *argv[])
{
	auto grid = new UnitTestGrid();
	grid->run();
	delete grid;
	/*
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();*/
}
