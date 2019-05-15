#pragma once

#include "PhysicsEngine.h"
#include <QOpenGLWidget>
#include <QBrush>
#include <QFont>
#include <QPen>
#include <QPainter>

class SimulationCanvasGLWidget : public QOpenGLWidget
{
	Q_OBJECT
public:
	SimulationCanvasGLWidget(QWidget *parent = Q_NULLPTR);

	~SimulationCanvasGLWidget();

	PhysicsEngine* engine;

	QBrush background;
	QBrush circleBrush;
	QPen circlePen;

protected:
	void paintEvent(QPaintEvent *event) override;

};

