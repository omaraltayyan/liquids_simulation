#include "SimulationCanvasGLWidget.h"

#include <QPaintEvent>
#include "Bodies\FluidParticle.h"

SimulationCanvasGLWidget::SimulationCanvasGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
	setAutoFillBackground(false);

	background = QBrush(QColor(0, 0, 0));
	circlePen = QPen(QColor(173, 216, 230));
	circlePen.setWidth(5);
	circleBrush = QBrush(circlePen.color());
	circlePen.setCapStyle(Qt::RoundCap);
}


SimulationCanvasGLWidget::~SimulationCanvasGLWidget()
{
}

void SimulationCanvasGLWidget::mousePressEvent(QMouseEvent *event) {
	QOpenGLWidget::mousePressEvent(event);
	addBodiesAtWidgetPosition(event->pos());
	// engine->pauseEngine();
}

void SimulationCanvasGLWidget::mouseMoveEvent(QMouseEvent *event) {
	QOpenGLWidget::mouseMoveEvent(event);

	addBodiesAtWidgetPosition(event->pos());
}

void SimulationCanvasGLWidget::mouseReleaseEvent(QMouseEvent * event)
{
	QOpenGLWidget::mouseReleaseEvent(event);
	// engine->resumeEngine();
}

void SimulationCanvasGLWidget::leaveEvent(QEvent *event)
{
	QOpenGLWidget::leaveEvent(event);
	// engine->resumeEngine();

}

void SimulationCanvasGLWidget::addBodiesAtWidgetPosition(QPointF position) {

	auto gridSize = engine->getUnsafeBodiesGrid().sizeInCentimeters();

	double xscaleFactor = gridSize.width() / this->width();
	double yscaleFactor = gridSize.height() / this->height();
	position.rx() *= xscaleFactor;
	position.ry() *= yscaleFactor;

	emitter->addRandomBodies(engine, emitter->particlesPerEmission, position);
}

void SimulationCanvasGLWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter;
	painter.begin(this);
	painter.setRenderHint(QPainter::Antialiasing, true);

    painter.fillRect(event->rect(), background);
	painter.setPen(circlePen);
	painter.setBrush(circleBrush);
	
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
