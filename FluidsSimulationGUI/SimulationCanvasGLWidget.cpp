#include "SimulationCanvasGLWidget.h"

#include <QPaintEvent>
#include "Bodies\FluidParticle.h"

SimulationCanvasGLWidget::SimulationCanvasGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
	setAutoFillBackground(false);

	background = QBrush(QColor(0, 0, 0));
	circlePen = QPen(QColor(173, 216, 230));
	circlePen.setWidth(2);
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
	
	auto gridSize = engine->getUnsafeBodiesGrid().sizeInCentimeters();

	engine->runFunctionOverBodies([&](Body* body) {
		FluidParticle* particle = dynamic_cast<FluidParticle*>(body);
		if (particle != NULL)
		{
			points.push_back(particle->position);
		}
	});
	
	double xscaleFactor = this->width() / gridSize.width();
	double yscaleFactor = this->height() / gridSize.height();

	for (int i = 0; i < points.length(); i++)
	{
		points[i].rx() *= xscaleFactor;
		points[i].ry() *= yscaleFactor;
	}

	painter.drawPoints(points);

	painter.end();
}
