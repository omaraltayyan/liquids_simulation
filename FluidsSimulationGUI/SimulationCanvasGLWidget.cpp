#include "SimulationCanvasGLWidget.h"

#include <QPaintEvent>
#include "Bodies\FluidParticle.h"

SimulationCanvasGLWidget::SimulationCanvasGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
	setAutoFillBackground(false);

	background = QBrush(QColor(0, 0, 0));
	circlePen = QPen(QColor(173, 216, 230));
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

	auto offset = containerOffsetPixels();
	position -= QPoint(offset.width(), offset.height());
	if (position.x() > containerWidth()) {
		return;
	}
	if (position.y() > containerHeight()) {
		return;
	}
	auto gridSize = engine->getUnsafeBodiesGrid().sizeInCentimeters();

	double xscaleFactor = gridSize.width() / containerWidth();
	double yscaleFactor = gridSize.height() / containerHeight();
	position.rx() *= xscaleFactor;
	position.ry() *= yscaleFactor;

	emitter->addRandomBodies(engine, emitter->particlesPerEmission, position);
}

int SimulationCanvasGLWidget::containerHeight()
{
	return this->height() - (containerOffsetPixels().height() * 2);
}

int SimulationCanvasGLWidget::containerWidth()
{
	return this->width() - (containerOffsetPixels().width() * 2);
}

const QSize SimulationCanvasGLWidget::containerOffsetPixels()
{
	auto gridSize = engine->getUnsafeBodiesGrid().sizeInCentimeters();

	double xscaleFactor = this->width() / gridSize.width();
	double yscaleFactor = this->height() / gridSize.height();

	return QSize(xscaleFactor * containerOffsetMeters, yscaleFactor * containerOffsetMeters);

}

void SimulationCanvasGLWidget::paintEvent(QPaintEvent *event)
{
	QVector<QPointF> points;
	points.reserve(engine->getBodiesCount());
	
	auto gridSize = engine->getUnsafeBodiesGrid().sizeInCentimeters();

	engine->runFunctionOverBodies([&](Body* body) {
		//FluidParticle* particle = dynamic_cast<FluidParticle*>(body);
		if (body->bodyType == fluid)
		{
			auto particle = (FluidParticle*)body;
			points.push_back(particle->position);
		}
	});
	double xscaleFactor = containerWidth() / gridSize.width();
	double yscaleFactor = containerHeight() / gridSize.height();
	
	double radiusScaleX = containerWidth() / gridSize.width();
	radiusScaleX *= radiusScaleX;
	double radiusScaleY = containerHeight() / gridSize.height();
	radiusScaleY *= radiusScaleY;
	double radiusScale = sqrt(radiusScaleX + radiusScaleY);
	circlePen.setWidth(emitter->emittedParticleRadius * radiusScale);
	for (int i = 0; i < points.length(); i++)
	{
		points[i].rx() *= xscaleFactor;
		points[i].ry() *= yscaleFactor;
	}
	auto offset = containerOffsetPixels();
	QPainter painter;
	painter.begin(this);
	painter.setRenderHint(QPainter::Antialiasing, true);

	painter.fillRect(event->rect(), background);
	painter.setPen(circlePen);
	painter.setBrush(circleBrush);
	painter.translate(offset.width(), offset.height());
	painter.drawPoints(points);

	painter.end();
}
