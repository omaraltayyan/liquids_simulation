#include "MainWindow.h"
#include "SimulationCanvasGLWidget.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	QTimer *timer = new QTimer(this);
	ui.openGLWidget->engine = &engine;
	ui.openGLWidget->emitter = &emitter;

	connect(timer, &QTimer::timeout, [=] {
		ui.openGLWidget->update();
		ui.statusBar->showMessage(QString::asprintf("Bodies: %d", engine.getBodiesCount()));
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
