#pragma once

#include "PhysicsEngine.h"
#include "RandomEmitter.h"
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
	RandomEmitter* emitter;

	QBrush background;
	QBrush circleBrush;
	QPen circlePen;

protected:
	bool resumeEngineOnRelease = false;
	int containerWidth();
	int containerHeight();

	const double containerOffsetMeters = 0.03;
	const QSize containerOffsetPixels();

	void paintEvent(QPaintEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void leaveEvent(QEvent *) override;

	void addBodiesAtWidgetPosition(QPointF position);
};

