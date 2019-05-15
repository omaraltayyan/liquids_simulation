#include "MainWindow.h"
#include "SimulationCanvasGLWidget.h"
#include <QTimer>
#include <windows.h>
#include <iostream>
#include <stdio.h>
using namespace std;

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

	connect(ui.SlownessSlider, &QSlider::valueChanged, [=] {
		this->engine.speedSlownessScale = ui.SlownessSlider->value();
	});

	connect(ui.ClearButton, &QPushButton::clicked, [=] {
		engine.clearBodies();
	});

	ui.ResumeButton->setText("Start");

	connect(ui.ResumeButton, &QPushButton::clicked, [=] {
		if (engine.isPaused()) {
			ui.ResumeButton->setText("Pause");
			engine.resumeEngine();
		}
		else {
			ui.ResumeButton->setText("Resume");
			engine.pauseEngine();
		}
	});

	linkLineEditWithValue(ui.ParticleMassText, &emitter.emittedParticleMass);
	linkLineEditWithValue(ui.ViscosityText, &emitter.emittedParticleViscosity);
	linkLineEditWithValue(ui.RestDensityText, &emitter.emittedParticleRestDensity);
	linkLineEditWithValue(ui.GasConstantText, &emitter.emittedParticleGasConstant);


	timer->start(1000 / 60);
}

void MainWindow::inputChanged() {
	this->updateDoubleValueWithText(ui.ParticleMassText->text(), &this->emitter.emittedParticleMass);
}

void MainWindow::linkLineEditWithValue(QLineEdit* lineEdit, double* doubleValue) {

	connect(lineEdit, &QLineEdit::textChanged, [=] {
		this->updateDoubleValueWithText(lineEdit->text(), doubleValue);
	});

	lineEdit->setText(QString::number(*doubleValue));
}

void MainWindow::updateDoubleValueWithText(QString text, double* doubleValue) {
	bool parsed;
	double newMass = text.toDouble(&parsed);
	if (parsed) {
		*doubleValue = newMass;
	}
}


void MainWindow::pauseClicked() {

}

void MainWindow::clearClicked() {

}
