#include "MainWindow.h"
#include "SimulationCanvasGLWidget.h"
#include <QTimer>
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include<qcolordialog.h>
using namespace std;

enum MaterialPresets
{

};

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
		QColor color = QColorDialog::getColor(emitter.currentMaterial.emittedParticleColor, this,"Choose Particle Color");
		
		if (color.isValid())
		{			
			emitter.currentMaterial.emittedParticleColor = color;
			this->updateUI();
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

	auto presets = QVector<Material>{
		water,
		mucus,
		steam,
	};

	ui.MaterialPresetsComboBox->addItem("Water");
	ui.MaterialPresetsComboBox->addItem("Mucus");
	ui.MaterialPresetsComboBox->addItem("Steam");

	connect(ui.MaterialPresetsComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
		emitter.currentMaterial = presets[index];
		this->updateUI();
	});

	connect(ui.surfaceTensionText, &QLineEdit::textChanged, [=] {
		this->updateDoubleValueWithText(ui.surfaceTensionText->text(), &emitter.currentMaterial.emittedParticleSurfaceTension);
	});

	connect(ui.thresholdText, &QLineEdit::textChanged, [=] {
		this->updateDoubleValueWithText(ui.thresholdText->text(), &emitter.currentMaterial.emittedParticleThreshold);
	});

	connect(ui.ParticleMassText, &QLineEdit::textChanged, [=] {
		this->updateDoubleValueWithText(ui.ParticleMassText->text(), &emitter.currentMaterial.emittedParticleMass);
	});

	connect(ui.ViscosityText, &QLineEdit::textChanged, [=] {
		this->updateDoubleValueWithText(ui.ViscosityText->text(), &emitter.currentMaterial.emittedParticleViscosity);
	});

	connect(ui.RestDensityText, &QLineEdit::textChanged, [=] {
		this->updateDoubleValueWithText(ui.RestDensityText->text(), &emitter.currentMaterial.emittedParticleRestDensity);
	});

	connect(ui.GasConstantText, &QLineEdit::textChanged, [=] {
		this->updateDoubleValueWithText(ui.GasConstantText->text(), &emitter.currentMaterial.emittedParticleGasConstant);
	});

	connect(ui.restitutionText, &QLineEdit::textChanged, [=] {
		this->updateDoubleValueWithText(ui.restitutionText->text(), &emitter.currentMaterial.emittedParticleRestitution);
	});

	connect(ui.BuoyancyText, &QLineEdit::textChanged, [=] {
		this->updateDoubleValueWithText(ui.BuoyancyText->text(), &emitter.currentMaterial.emittedParticleBuoyancy);
	});

	ui.CollisionObjectsComboBox->addItem("Box", COLLISION_OBJECT_BOX);
	ui.CollisionObjectsComboBox->addItem("Capsule", COLLISION_OBJECT_CAPSULE);
	ui.CollisionObjectsComboBox->addItem("Sphere", COLLISION_OBJECT_SPHERE);


	connect(ui.CollisionObjectsComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
		auto newObject = static_cast<CollisionObject>(ui.CollisionObjectsComboBox->itemData(index).toInt());
		engine.setCollisionObject(newObject);
	});
	this->updateUI();

	timer->start(1000 / 60);
}

void MainWindow::updateUI() {
	ui.surfaceTensionText->setText(QString::number(emitter.currentMaterial.emittedParticleSurfaceTension));
	ui.thresholdText->setText(QString::number(emitter.currentMaterial.emittedParticleThreshold));
	ui.ParticleMassText->setText(QString::number(emitter.currentMaterial.emittedParticleMass));
	ui.ViscosityText->setText(QString::number(emitter.currentMaterial.emittedParticleViscosity));
	ui.RestDensityText->setText(QString::number(emitter.currentMaterial.emittedParticleRestDensity));
	ui.GasConstantText->setText(QString::number(emitter.currentMaterial.emittedParticleGasConstant));
	ui.restitutionText->setText(QString::number(emitter.currentMaterial.emittedParticleRestitution));
	ui.BuoyancyText->setText(QString::number(emitter.currentMaterial.emittedParticleBuoyancy));

	QPalette pallete = ui.colorSelectButton->palette();
	pallete.setColor(QPalette::Button, emitter.currentMaterial.emittedParticleColor);
	ui.colorSelectButton->setAutoFillBackground(true);
	ui.colorSelectButton->setPalette(pallete);
	ui.colorSelectButton->update();

}


void MainWindow::linkLineEditWithValue(QLineEdit* lineEdit, double* doubleValue) {
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
