#include "Particle.h"

#define SQUARE_ROOT_OF_2 1.41421356237

Particle::Particle(const QPointF& position, qreal sizeRadius) : MovingBody(QRectF(position.x(), position.y(), sizeRadius * SQUARE_ROOT_OF_2, sizeRadius * SQUARE_ROOT_OF_2))
{
	updatePosition();
}

void Particle::setPosition(const QPointF& newPosition) {
	this->boundingRect.moveCenter(newPosition);
	updatePosition();
}

void Particle::setPosition(qreal x, qreal y) {
	this->setPosition(QPointF(x, y));
}

void Particle::updatePosition() {
	this->sizeRadius = this->boundingRect.width() / SQUARE_ROOT_OF_2;
	this->position = this->boundingRect.center();
	this->positionVector = QVector2D(this->position);
}
