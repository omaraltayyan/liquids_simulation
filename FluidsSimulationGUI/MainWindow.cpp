#include "MainWindow.h"
#include "SimulationCanvasGLWidget.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	QTimer *timer = new QTimer(this);
	ui.openGLWidget->engine = &engine;
	connect(timer, &QTimer::timeout, [=] {
		emitter.addRandomBodies(&engine, 50);
		ui.openGLWidget->update();
	});

	timer->start(1000 / 60);

	engine.resumeEngine();
}

void MainWindow::drawingWindowClicked() {

}

void MainWindow::inputChanged() {

}

void MainWindow::pauseClicked() {

}

void MainWindow::clearClicked() {

}
