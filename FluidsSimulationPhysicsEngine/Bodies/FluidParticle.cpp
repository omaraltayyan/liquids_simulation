#include "FluidParticle.h"
#include "PhysicsEngine.h"
#include <qmath.h>

FluidParticle::FluidParticle(const QPointF& position, PhysicsEngine* engine,
	double viscosity, double mass, double gasConstant, double restDesity,
	double surfaceTension, double threshold, double restitution, double buoyancy, QColor color, double radius) : Particle(position, engine)
{
	_buoyancy = buoyancy;
	_restitution = restitution;
	_viscosity = viscosity;
	_mass = mass;
	_gasConstant = gasConstant;
	_restDensity = restDesity;
	_velocity.setX(0);
	_velocity.setY(0);

	_radius = radius;
	_kernelsInfo = KernelsInfo(radius);

	_accelration.setX(0.0);
	_accelration.setY(0.0);

	_tensionCoefcioant = surfaceTension;
	_surfaceThreshold = threshold;
	_isFirstIteration = true;
	this->bodyType = fluid;
	this->particleColor = color;
	this->displayRadius = 1.5*cbrt(3.0 * this->_mass / (4.0 * M_PI * this->_restDensity));
}


double FluidParticle::applyKernal(double distance, SmoothingKernals kernal)
{
	if (kernal == poly6 || kernal == gradPoly6 || kernal == lapPoly6) {
		double holder = _kernelsInfo.radius_2 - (distance * distance);
		switch (kernal)
		{
		case poly6:
			return _kernelsInfo.poly6Left * (holder*holder*holder);
		case gradPoly6:
			return _kernelsInfo.gradAndLapPoly6Left * (holder*holder);
		case lapPoly6:
			return _kernelsInfo.gradAndLapPoly6Left * holder * ((3 * _kernelsInfo.radius_2) - (7 * distance*distance));
		}
	}

	switch (kernal)
	{
	case spiky:
		return _kernelsInfo.spikyLeft * ((_kernelsInfo.radius - distance) * (_kernelsInfo.radius - distance));
	case visc:
		return _kernelsInfo.viscLeft * (_kernelsInfo.radius - distance);
	}

	return 0.0;


}

double FluidParticle::computeDensity(const QVector<BodiesVector*>& surroundingBodies, double radius)
{
	double resultingDynsity = 0.0;
	this->runFunctionOverFluidParicles(surroundingBodies, radius, [&](FluidParticle* particle, double distance) {
		resultingDynsity += particle->_mass * this->applyKernal(distance, poly6);
	});
	return resultingDynsity;
}

double FluidParticle::computePressure()
{
	return _gasConstant  * (_density - _restDensity);
}

QCPVector2D FluidParticle::computePressureForce(const QVector<BodiesVector*>& surroundingBodies, double radius)
{
	QCPVector2D resultingPressureForce(0.0, 0.0);
	auto relativePressureTerm = this->_pressure / (this->_density * this->_density);
	this->runFunctionOverFluidParicles(surroundingBodies, radius, [&](FluidParticle* particle, double distance) {
		if (particle == this)
			return;
		auto leftTerm = (relativePressureTerm + (particle->_pressure / (particle->_density * particle->_density))) * particle->_mass;
		if (distance == 0.) {
			auto randomVector = MathUtilities::randomVector(0.0001, -0.0001);
			resultingPressureForce += leftTerm * (randomVector * this->engine->getUnsafeBodiesGrid().getKernelsInfo().spikyLeft);
		}
		else {
			auto vec = this->positionVector - particle->positionVector;
			vec.normalize();
			resultingPressureForce += vec * leftTerm * this->applyKernal(distance, spiky);
		}
	});
	return (-this->_density) *  resultingPressureForce;
}

QCPVector2D FluidParticle::computeViscousForce(const QVector<BodiesVector*>& surroundingBodies, double radius)
{
	QCPVector2D resultingVisousForce(0, 0);
	this->runFunctionOverFluidParicles(surroundingBodies, radius, [&](FluidParticle* particle, double distance) {
		if (particle == this)
			return;
		resultingVisousForce += ((particle->_velocity - this->_velocity) / particle->_density) * particle->_mass
			* this->applyKernal(distance, visc);
	});

	return this->_viscosity * resultingVisousForce;
}

//this method will compute the surface normal(which is the gradient to the color field)
QCPVector2D FluidParticle::computeSurfaceNormal(const QVector<BodiesVector*>& surroundingBodies, double radius)
{
	QCPVector2D resultingSurfaceNormal(0.0, 0.0);
	this->runFunctionOverFluidParicles(surroundingBodies, radius, [&](FluidParticle* particle, double distance) {
		auto vec = this->positionVector - particle->positionVector;
		double kernel = this->applyKernal(distance, gradPoly6);
		resultingSurfaceNormal += vec * (particle->_mass / particle->_density)*kernel;
	});
	return resultingSurfaceNormal;
}


//this method we compute the laplacian of the color field to use it to compute the surface tension
double FluidParticle::computeLaplacianSurfaceNormal(const QVector<BodiesVector*>& surroundingBodies, double radius)
{
	double resultingSurfaceNormal = 0.0;

	this->runFunctionOverFluidParicles(surroundingBodies, radius, [&](FluidParticle* particle, double distance) {
		double kernel = this->applyKernal(distance, lapPoly6);
		resultingSurfaceNormal += (particle->_mass / particle->_density)*kernel;
	});

	return resultingSurfaceNormal;
}

QCPVector2D FluidParticle::computeSurfaceTension(const QVector<BodiesVector*>& surroundingBodies, double radius)
{
	QCPVector2D resultingSurfaceTension(0.0, 0.0);
	if (this->_tensionCoefcioant > 0) {

		auto surfaceNormal = this->computeSurfaceNormal(surroundingBodies, radius);
		double magnitude = surfaceNormal.length();
		if (magnitude >= this->_surfaceThreshold)
		{
			surfaceNormal.normalize();
			resultingSurfaceTension = this->computeLaplacianSurfaceNormal(surroundingBodies, radius) * surfaceNormal;
		}
	}

	return -1 * this->_tensionCoefcioant * resultingSurfaceTension;
}

void FluidParticle::runFunctionOverFluidParicles(const QVector<BodiesVector*>& surroundingBodies, double radius, const function <void(FluidParticle*, double)>&& func)
{
	for (int i = 0; i < surroundingBodies.length(); i++)
	{
		auto bodyVector = surroundingBodies[i];
		for (int j = 0; j < bodyVector->length(); j++)
		{
			auto body = bodyVector->at(j);
			if (body->bodyType == fluid)
			{
				auto particle = static_cast<FluidParticle*>(body);
				double distance = this->positionVector.distanceToPoint(particle->positionVector);
				if (distance < radius) {
					func(particle, distance);
				}
			}
		}
	}
}

QCPVector2D FluidParticle::computeSumOfForces(const QVector<BodiesVector*>& surroundingBodies, double radius)
{
	QCPVector2D pressureForce = this->computePressureForce(surroundingBodies, radius);
	QCPVector2D viscousForce = this->computeViscousForce(surroundingBodies, radius);
	QCPVector2D externalForce;
	if (this->_buoyancy > 0) {
		externalForce = this->engine->getGravity() * this->_buoyancy * (this->_density - this->_restDensity);
	}
	else {
		externalForce = this->engine->getGravity() * this->_density;
	}
	externalForce += this->computeSurfaceTension(surroundingBodies, radius);
	return pressureForce + viscousForce + externalForce;
}

void FluidParticle::detectCollisionWithASquare(const QRectF& boundingBox)
{
	int collisionSides = 0;

	auto marginBoundingBox = boundingBox.marginsRemoved(QMarginsF(0.01, 0.01, 0.01, 0.01));

	auto center = QCPVector2D(marginBoundingBox.center());

	auto extentVector = QCPVector2D(marginBoundingBox.width() / 2.0, marginBoundingBox.height() / 2.0);

	auto pointInLocalCoords = this->positionVector - center;

	auto absolutePoint = MathUtilities::abs(pointInLocalCoords);

	auto boxFunction = MathUtilities::max(absolutePoint - extentVector);

	if (boxFunction <= 0) {
		return;
	}

	QCPVector2D normal(0, 0);

	if (pointInLocalCoords.x() > extentVector.x()) {
		pointInLocalCoords.setX(extentVector.x());
		normal.setX(-1);
		collisionSides += 1;
	}
	else if (pointInLocalCoords.x() < -extentVector.x()) {
		pointInLocalCoords.setX(-extentVector.x());
		normal.setX(1);
		collisionSides += 1;
	}

	if (pointInLocalCoords.y() > extentVector.y()) {
		pointInLocalCoords.setY(extentVector.y());
		normal.setY(-1);
		collisionSides += 1;
	}
	else if (pointInLocalCoords.y() < -extentVector.y()) {
		pointInLocalCoords.setY(-extentVector.y());
		normal.setY(1);
		collisionSides += 1;
	}

	auto contactPoint = pointInLocalCoords + center;

	auto penetrationDepth = (contactPoint - this->positionVector).length();

	normal.normalize();

	/*
	auto contactPointLocalCoords = QCPVector2D(max(-1 * extentVector.x(), pointInLocalCoords.x()),
	max(-1 * extentVector.y(), pointInLocalCoords.y()));

	contactPointLocalCoords = QCPVector2D(max(extentVector.x(), contactPointLocalCoords.x()),
	max(extentVector.y(), contactPointLocalCoords.y()));

	auto contactPoint = contactPointLocalCoords + center;

	auto penetrationDepth = (contactPoint - this->positionVector).length();

	auto normalVector = contactPointLocalCoords - pointInLocalCoords;

	auto normal = QCPVector2D(signumFunction(normalVector.x()),
	signumFunction(normalVector.y())).normalized();

	int collisionSides = 0;

	if (this->positionVector.x() < 0) {
	collisionSides += 1;
	}
	if (this->positionVector.y() < 0) {
	collisionSides += 1;
	}
	if (this->positionVector.x() > boundingBox.width()) {
	collisionSides += 1;
	}
	if (this->positionVector.y() > boundingBox.height()) {
	collisionSides += 1;
	}*/

	/*
	auto rightSide = QCPVector2D(qMax(0.0, this->positionVector.x()), qMax(0.0, this->positionVector.y()));

	auto contactPoint = QCPVector2D(qMin(boundingBox.width(), rightSide.x()), qMin(boundingBox.height(), rightSide.y()));

	auto penetrationDepth = this->positionVector.distanceToPoint(contactPoint);
	if (penetrationDepth <= 0)
	{
	return;
	}
	QCPVector2D normal;

	int collisionSides = 0;

	if (this->positionVector.x() < 0) {
	collisionSides += 1;
	normal.rx() += 1;
	}
	if (this->positionVector.y() < 0) {
	collisionSides += 1;
	normal.ry() += 1;
	}
	if (this->positionVector.x() > boundingBox.width()) {
	collisionSides += 1;
	normal.rx() -= 1;
	}
	if (this->positionVector.y() > boundingBox.height()) {
	collisionSides += 1;
	normal.ry() -= 1;
	}
	normal.normalize();

	contactPoint += penetrationDepth * normal;
	*/

	// two sides collided, add ad random translation toward the 
	// center to avoid chaos in the simulation
	if (collisionSides > 1) {

		QCPVector2D randomMovement = MathUtilities::randomVector(0.0001);
		randomMovement.rx() *= normal.x();
		randomMovement.ry() *= normal.y();
		contactPoint += randomMovement;
	}

	this->applyCollision(contactPoint, normal, penetrationDepth);
}

void FluidParticle::detectCollisionWithACapsule(const QRectF& boundingBox, double cylinder_width) {

	// the width (radius) of the capsule must never exceed half the 
	// width nor the height of the bounding box
	cylinder_width = min(cylinder_width, boundingBox.width() / 2.0);
	cylinder_width = min(cylinder_width, boundingBox.height() / 2.0);

	auto capsule_p0 = QCPVector2D(boundingBox.width() / 2.0, cylinder_width + 0.001);
	auto capsule_p1 = QCPVector2D(boundingBox.width() / 2.0, boundingBox.height() - cylinder_width - 0.001);

	auto pointsDifference = capsule_p1 - capsule_p0;
	auto pointsDifferenceVector = QCPVector2D(pointsDifference);
	auto differenceLengthSquared = pointsDifferenceVector.lengthSquared();

	// the innerMostValue in the capsule equation
	auto innerValue = -1 * QCPVector2D(capsule_p0 - this->position).dot(pointsDifferenceVector) / differenceLengthSquared;

	auto equationRightSide = min(1.0, max(0.0, innerValue)) * pointsDifference;

	auto equationQValue = capsule_p0 + equationRightSide;

	auto capsuleFunction = QCPVector2D(equationQValue - this->position).length() - cylinder_width;

	if (capsuleFunction <= 0) {
		return;
	}

	auto normalizedQFromPosition = (this->position - equationQValue).normalized();

	auto contactPoint = equationQValue + (cylinder_width * (normalizedQFromPosition));

	auto penetrationDepth = abs(capsuleFunction);

	auto normal = signumFunction(capsuleFunction) * normalizedQFromPosition;
	//contactPoint = contactPoint - penetrationDepth * normal;

	this->applyCollision(contactPoint, normal, penetrationDepth);
}

void FluidParticle::detectCollisionWithASphere(const QRectF& boundingBox) {
	// the width (radius) of the capsule must never exceed half the 
	// width nor the height of the bounding box
	double radius = min(boundingBox.height() / 2.0, boundingBox.width() / 2.0);

	auto circuleCenter = QPointF(boundingBox.width() / 2.0, boundingBox.height() / 2.0);

	auto circleFunction = QCPVector2D(circuleCenter - this->position).length() - radius;

	if (circleFunction <= 0) {
		return;
	}

	auto normalizedQFromPosition = (this->position - circuleCenter) / QCPVector2D(this->position - circuleCenter).length();

	auto contactPoint = circuleCenter + (radius * (normalizedQFromPosition));

	auto penetrationDepth = abs(circleFunction);

	auto normal = signumFunction(circleFunction) * normalizedQFromPosition;
	//contactPoint = contactPoint - penetrationDepth * normal;

	this->applyCollision(contactPoint, normal, penetrationDepth);

}

void FluidParticle::applyCollision(const QCPVector2D & contactPoint, const QCPVector2D& collisionSurfaceNormal, double penterationDepth) {

	this->setPosition(contactPoint.toPointF());

	const double restitutionTerm = this->_restitution > 0 ? this->_restitution * (penterationDepth / (this->engine->getTimeDelta() * this->_leapFrogNextStep.length())) : 0;

	this->_leapFrogNextStep -= (1 + restitutionTerm) * this->_leapFrogNextStep.dot(collisionSurfaceNormal) * collisionSurfaceNormal;
}

int FluidParticle::signumFunction(double x)
{
	if (x == 0.0)
		return 0;
	if (x < 0.0)
		return -1;
	return 1;
}

/*
important note about threading:
first density should computed for all particles
second pressure should be computed for all particles
then the rest can work simultaneously
*/
void FluidParticle::calculateInteractionWithBodies(const QVector<BodiesVector*>& surroundingBodies, int calculationOperation)
{
	// note that density and pressure needs to be computed here
	// because their values are important in the other methods
	auto engine = PhysicsEngine::shared();
	if (calculationOperation == 0) {
		this->_density = this->computeDensity(surroundingBodies, _radius);
		//here gas constant and rest density should variables taken from user input
		this->_pressure = this->computePressure();
	}
	else if (calculationOperation == 1) {
		//here gravity should also be taken from user input
		this->_force = this->computeSumOfForces(surroundingBodies, _radius);
	}
}

void FluidParticle::applyInteraction()
{
	auto accelration = this->_force / this->_density;
	if (!this->_isFirstIteration)
	{
		auto size = this->engine->getUnsafeBodiesGrid().sizeInMeters();
		auto boundingBox = QRectF(0.0, 0.0, size.width(), size.height());
		auto previousStep = QCPVector2D(this->_leapFrogNextStep);
		this->_leapFrogNextStep += engine->getTimeDelta() * accelration;
		this->setPosition((this->positionVector + this->_leapFrogNextStep * engine->getTimeDelta()).toPointF());

		switch (engine->getCollisionObject())
		{
		case COLLISION_OBJECT_BOX:
			this->detectCollisionWithASquare(boundingBox);
			break;
		case COLLISION_OBJECT_CAPSULE:
			this->detectCollisionWithACapsule(boundingBox, 0.5);
			break;
		case COLLISION_OBJECT_SPHERE:
			this->detectCollisionWithASphere(boundingBox);
			break;
		}
		this->_velocity = (previousStep + this->_leapFrogNextStep) * 0.5;
	}
	else
	{
		this->_leapFrogNextStep = this->_velocity - (0.5 * engine->getTimeDelta() * accelration);
		this->_isFirstIteration = false;
	}
	this->displayRadius = 1.5*cbrt(3.0 * this->_mass / (4.0 * M_PI * this->_density));
}


