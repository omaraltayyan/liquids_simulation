#include "MainWindow.h"
#include <QtWidgets/QApplication>
#include "UnitTests\UnitTestGrid.h"
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
	/*auto grid = new UnitTestGrid();
	grid->run();
	delete grid;*/
	
	QApplication a(argc, argv);

    QSurfaceFormat fmt;
    fmt.setSamples(4);
    QSurfaceFormat::setDefaultFormat(fmt);

	MainWindow w;
	w.show();
	return a.exec();
}
