#include "Particle.h"

#define SQUARE_ROOT_OF_2 1.41421356237

Particle::Particle(const QPointF& position, PhysicsEngine* engine)
	: MovingBody(QRectF(position.x(), position.y(), 1e-10,1e-10), engine)
{
	this->position = this->boundingRect.center();
	updatePosition();
}

void Particle::setPosition(const QPointF& newPosition) {
	this->position = newPosition;
	this->boundingRect.moveCenter(newPosition);
	updatePosition();
}

void Particle::setPosition(qreal x, qreal y) {
	this->setPosition(QPointF(x, y));
}

void Particle::updatePosition() {
	this->positionVector = QCPVector2D(this->position);
}
