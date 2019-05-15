#include "Body.h"

Body::Body(const Body& p) : Body(p.boundingRect, p.engine) {
}

Body::Body(QRectF boundingRect, PhysicsEngine* engine): boundingRect(boundingRect) {
	this->engine = engine;
}

Body::Body() {

}
