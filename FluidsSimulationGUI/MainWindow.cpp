#include "MainWindow.h"
#include "SimulationCanvasGLWidget.h"
#include <QTimer>
#include <windows.h>
#include <iostream>
#include <stdio.h>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//AllocConsole();
	//freopen("conin$", "r", stdin);
	//freopen("conout$", "w", stdout);
	//freopen("conout$", "w", stderr);

	QTimer *timer = new QTimer(this);
	ui.openGLWidget->engine = &engine;
	ui.openGLWidget->emitter = &emitter;

	connect(timer, &QTimer::timeout, [=] {
		ui.openGLWidget->update();
		ui.statusBar->showMessage(QString::asprintf("FPS: %.1f Bodies: %d", engine.fps, engine.bodiesCount));
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
