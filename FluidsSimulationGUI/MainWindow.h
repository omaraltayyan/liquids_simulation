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

private slots:
void drawingWindowClicked();
void inputChanged();
void pauseClicked();
void clearClicked();

};
