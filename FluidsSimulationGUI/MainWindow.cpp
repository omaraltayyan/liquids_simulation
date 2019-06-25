#include "MainWindow.h"
#include "SimulationCanvasGLWidget.h"
#include <QTimer>
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include<qcolordialog.h>
using namespace std;

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

#ifdef _DEBUG
	AllocConsole();
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);
#endif
	QTimer *timer = new QTimer(this);
	ui.openGLWidget->engine = &engine;
	ui.openGLWidget->emitter = &emitter;

	connect(timer, &QTimer::timeout, [=] {
		ui.openGLWidget->update();
		ui.statusBar->showMessage(QString::asprintf("FPS: %.1f Bodies: %d timeDelta: %.2fms Gravity: %.1fm/s Speed: %.2fx",
			engine.fps, engine.bodiesCount, engine.getTimeDelta() * 1000, engine.getGravity().y(), 1.0 / engine.speedSlownessScale));		
	});
	const double valueOnSliderValueOfOne = 0.001;
	const double newSliderValue = this->engine.getTimeDelta() / valueOnSliderValueOfOne;
	ui.timeDeltaSlider->setValue(newSliderValue);
	ui.gravitySlider->setValue(this->engine.getGravity().y());
	ui.SlownessSlider->setValue(this->engine.speedSlownessScale);
	ui.emissionSlider->setValue(this->emitter.particlesPerEmission);

	connect(ui.timeDeltaSlider, &QSlider::valueChanged, [=] {
		this->engine.setTimeDelta(valueOnSliderValueOfOne * ui.timeDeltaSlider->value());
	});

	connect(ui.gravitySlider, &QSlider::valueChanged, [=] {
		this->engine.setGravity(QCPVector2D(0, ui.gravitySlider->value()));
	});

	connect(ui.emissionSlider, &QSlider::valueChanged, [=] {
		this->emitter.particlesPerEmission = ui.emissionSlider->value();
	});

	connect(ui.SlownessSlider, &QSlider::valueChanged, [=] {
		this->engine.speedSlownessScale = ui.SlownessSlider->value();
	});

	connect(ui.colorSelectButton, &QPushButton::clicked, [=] {
		QColor color = QColorDialog::getColor(Qt::cyan, this,"Choose Particle Color");
		
		if (color.isValid())
		{			
			
			QPalette pallete = ui.colorSelectButton->palette();
			pallete.setColor(QPalette::Button,color);
			ui.colorSelectButton->setAutoFillBackground(true);
			ui.colorSelectButton->setPalette(pallete);
			ui.colorSelectButton->update();		
			emitter.emittedParticleColor = color;
			
		}
	});

	connect(ui.pauseOnSpawnCheckbox, &QCheckBox::clicked, [=] {
		ui.openGLWidget->pauseEngineOnSpawn = ui.pauseOnSpawnCheckbox->isChecked();
	});

	connect(ui.ClearButton, &QPushButton::clicked, [=] {
		engine.clearBodies();
	});

	connect(ui.ResumeButton, &QPushButton::clicked, [=] {
		if (engine.isPaused()) {
			ui.ResumeButton->setText("Pause");
			ui.ResumeButton->setShortcut(QKeySequence(Qt::Key_R));
			engine.resumeEngine();
		}
		else {
			ui.ResumeButton->setText("Resume");
			ui.ResumeButton->setShortcut(QKeySequence(Qt::Key_R));
			engine.pauseEngine();
		}
	});

	linkLineEditWithValue(ui.surfaceTensionText, &emitter.emittedParticleSurfaceTension);
	linkLineEditWithValue(ui.thresholdText, &emitter.emittedParticleThreshold);
	linkLineEditWithValue(ui.ParticleMassText, &emitter.emittedParticleMass);
	linkLineEditWithValue(ui.ViscosityText, &emitter.emittedParticleViscosity);
	linkLineEditWithValue(ui.RestDensityText, &emitter.emittedParticleRestDensity);
	linkLineEditWithValue(ui.GasConstantText, &emitter.emittedParticleGasConstant);
	linkLineEditWithValue(ui.restitutionText, &emitter.emittedParticleRestitution);
	linkLineEditWithValue(ui.BuoyancyText, &emitter.emittedParticleBuoyancy);
	
	ui.CollisionObjectsComboBox->addItem("Box", COLLISION_OBJECT_BOX);
	ui.CollisionObjectsComboBox->addItem("CAPSULE", COLLISION_OBJECT_CAPSULE);
	ui.CollisionObjectsComboBox->addItem("SPHERE", COLLISION_OBJECT_SPHERE);


	connect(ui.CollisionObjectsComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
		auto newObject = static_cast<CollisionObject>(ui.CollisionObjectsComboBox->itemData(index).toInt());
		engine.setCollisionObject(newObject);
	});

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
