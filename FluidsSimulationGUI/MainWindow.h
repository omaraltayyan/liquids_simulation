#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "RandomEmitter.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);

private:
	Ui::MainWindowClass ui;

	PhysicsEngine engine;
	RandomEmitter emitter;

	void updateUI();
	void linkLineEditWithValue(QLineEdit* lineEdit, double * doubleValue);
	void updateDoubleValueWithText(QString text, double * doubleValue);
	void pauseClicked();
	void clearClicked();

};
