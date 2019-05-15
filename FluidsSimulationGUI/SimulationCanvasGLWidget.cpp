#include "SimulationCanvasGLWidget.h"

#include <QPaintEvent>
#include "Bodies\FluidParticle.h"

SimulationCanvasGLWidget::SimulationCanvasGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
	setAutoFillBackground(false);

	background = QBrush(QColor(100, 100, 100));
	circleBrush = QBrush(QColor(255, 255, 255));
	circlePen = QPen(Qt::black);
	circlePen.setWidth(10);
}


SimulationCanvasGLWidget::~SimulationCanvasGLWidget()
{
}

void SimulationCanvasGLWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter;
	painter.begin(this);
	painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(event->rect(), background);
    painter.setBrush(circleBrush);
    painter.setPen(circlePen);
	
	QVector<QPointF> points;
	points.reserve(engine->getBodiesCount());

	engine->runFunctionOverBodies([&](Body* body) {
		FluidParticle* particle = dynamic_cast<FluidParticle*>(body);
		if (particle != NULL)
		{
			points.push_back(particle->position);
		}
	});
	painter.drawPoints(points);

	painter.end();
}
