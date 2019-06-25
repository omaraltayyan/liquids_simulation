#include "FluidParticle.h"
#include "PhysicsEngine.h"
#include <qmath.h>

FluidParticle::FluidParticle(const QPointF& position, PhysicsEngine* engine,
	double viscosity, double mass, double gasConstant, double restDesity,
	double surfaceTension, double threshold, double restitution, double buoyancy,QColor color) : Particle(position, engine)
{
	_buoyancy = buoyancy;
	_restitution = restitution;
	_viscosity = viscosity;
	_mass = mass;
	_gasConstant = gasConstant;
	_restDensity = restDesity;
	_velocity.setX(0.0);
	_velocity.setY(0.0);

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
	const KernelsInfo& kernelsInfo = this->engine->getUnsafeBodiesGrid().getKernelsInfo();

	if (kernal == poly6 || kernal == gradPoly6 || kernal == lapPoly6) {
		double holder = kernelsInfo.radius_2 - (distance * distance);
		switch (kernal)
		{
		case poly6:
			return kernelsInfo.poly6Left * (holder*holder*holder);
		case gradPoly6:
			return kernelsInfo.gradAndLapPoly6Left * (holder*holder);
		case lapPoly6:
			return kernelsInfo.gradAndLapPoly6Left * holder * ((3 * kernelsInfo.radius_2) - (7 * distance*distance));
		}
	}

	switch (kernal)
	{
	case spiky:
		return kernelsInfo.spikyLeft * ((kernelsInfo.radius - distance) * (kernelsInfo.radius - distance));
	case visc:
		return kernelsInfo.viscLeft * (kernelsInfo.radius - distance);
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
		if (particle == this || MathUtilities::isEqual(distance, 0))
			return;
		auto vec = this->positionVector - particle->positionVector;
		vec.normalize();
		auto leftTerm = (relativePressureTerm + (particle->_pressure / (particle->_density * particle->_density))) * particle->_mass;
		resultingPressureForce += vec * leftTerm * this->applyKernal(distance, spiky);
	});
	return (-this->_density) *  resultingPressureForce;
}

QCPVector2D FluidParticle::computeViscousForce(const QVector<BodiesVector*>& surroundingBodies, double radius)
{
	QCPVector2D resultingVisousForce(0, 0);
	this->runFunctionOverFluidParicles(surroundingBodies, radius, [&](FluidParticle* particle, double distance) {
		if (particle == this || MathUtilities::isEqual(distance, 0))
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
	auto surfaceNormal = this->computeSurfaceNormal(surroundingBodies, radius);
	double magnitude = surfaceNormal.length();
	if (magnitude >= this->_surfaceThreshold)
	{
		surfaceNormal.normalize();
		resultingSurfaceTension = this->computeLaplacianSurfaceNormal(surroundingBodies, radius) * surfaceNormal;
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
				if (distance <= radius) {
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
		externalForce += this->computeSurfaceTension(surroundingBodies, radius);
	}
	return pressureForce + viscousForce + externalForce;
}

void FluidParticle::detectCollisionWithASquare(const QRectF& boundingBox)
{
	if (boundingBox.contains(this->position))
	{
		return;
	}

	QCPVector2D extentVector = QCPVector2D(boundingBox.width(), boundingBox.height());

	auto rightSide = QCPVector2D(qMax(0.0, this->positionVector.x()), qMax(0.0, this->positionVector.y()));

	auto contactPoint = QCPVector2D(qMin((extentVector).x(), rightSide.x()), qMin((extentVector).y(), rightSide.y()));

	auto penterationDepth = this->positionVector.distanceToPoint(contactPoint);
	if (penterationDepth <= 0)
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
	if (this->positionVector.x() >= extentVector.x()) {
		collisionSides += 1;
		normal.rx() -= 1;
	}
	if (this->positionVector.y() >= extentVector.y()) {
		collisionSides += 1;
		normal.ry() -= 1;
	}

	normal.normalize();

	// two sides collided, add ad random translation toward the 
	// center to avoid chaos in the simulation
	if (collisionSides >= 2) {
		float randomDistanceX = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 0.0001));
		float randomDistanceY = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 0.0001));

		QCPVector2D randomMovement = QCPVector2D();
		randomMovement.setX(normal.x() * randomDistanceX);
		randomMovement.setY(normal.y() * randomDistanceY);
		contactPoint += randomMovement;
	}

	this->applyCollision(contactPoint, normal, penterationDepth);
}

void FluidParticle::detectCollisionWithACapsule(const QRectF& boundingBox, double cylinder_width) {

	// the width (radius) of the capsule must never exceed half the 
	// width nor the height of the bounding box
	cylinder_width = min(cylinder_width, boundingBox.width() / 2.0);
	cylinder_width = min(cylinder_width, boundingBox.height() / 2.0);
	
	auto capsule_p0 = QPointF(boundingBox.width() / 2.0, cylinder_width);
	auto capsule_p1 = QPointF(boundingBox.width() / 2.0, boundingBox.height() - cylinder_width);

	auto pointsDifference = capsule_p1 - capsule_p0;
	auto pointsDifferenceVector = QCPVector2D(pointsDifference);
	auto differenceLengthSquared = pointsDifferenceVector.lengthSquared();

	// the innerMostValue in the capsule equation
	auto innerValue = -1 * QCPVector2D(capsule_p0 - this->position).dot(pointsDifferenceVector) / differenceLengthSquared;

	auto equationRightSide = min(1.0, max(0.0, innerValue)) * pointsDifference;

	auto equationQValue = capsule_p0 + equationRightSide;

	auto capsuleFunction = QCPVector2D(equationQValue - this->position).length() - cylinder_width;

	if (capsuleFunction < 0) {
		return;
	}

	auto normalizedQFromPosition = (this->position - equationQValue) / QCPVector2D(this->position - equationQValue).length();

	auto contactPoint = equationQValue + (cylinder_width * (normalizedQFromPosition));

	auto penetrationDepth = abs(capsuleFunction);

	auto normal = signumFunction(capsuleFunction) * normalizedQFromPosition;

	this->applyCollision(contactPoint, normal, penetrationDepth);
}

void FluidParticle::applyCollision(const QCPVector2D & contactPoint, const QCPVector2D& collisionSurfaceNormal, double penterationDepth) {

	this->setPosition(contactPoint.toPointF());

	const double restitutionTerm = this->_restitution > 0 ? this->_restitution * (penterationDepth / (this->engine->getTimeDelta() * this->_leapFrogNextStep.length())) : 0;

	this->_leapFrogNextStep -= (1 + restitutionTerm) * this->_leapFrogNextStep.dot(collisionSurfaceNormal) * collisionSurfaceNormal;
}

int FluidParticle::signumFunction(double x)
{
	if (MathUtilities::isEqual(x, 0.0))
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
	double radius = engine->getUnsafeBodiesGrid().squareSideInMeters();

	if (calculationOperation == 0) {
		this->_density = this->computeDensity(surroundingBodies, radius);
		//here gas constant and rest density should variables taken from user input
		this->_pressure = this->computePressure();
	}
	else if (calculationOperation == 1) {
		//here gravity should also be taken from user input
		this->_force = this->computeSumOfForces(surroundingBodies, radius);
	}
}

void FluidParticle::applyInteraction()
{
	auto accelration = this->_force / this->_density;
	if (!this->_isFirstIteration)
	{
		auto previousStep = QCPVector2D(this->_leapFrogNextStep);
		this->_leapFrogNextStep += engine->getTimeDelta() * accelration;
		this->positionVector += this->_leapFrogNextStep * engine->getTimeDelta();
		this->setPosition(this->positionVector.toPointF());

		auto size = this->engine->getUnsafeBodiesGrid().sizeInMeters();
		if (!MathUtilities::isEqual(this->_leapFrogNextStep.lengthSquared(), 0.0)) {
			 //this->detectCollisionWithASquare(QRectF(0.0, 0.0, size.width(), size.height()));
			this->detectCollisionWithACapsule(QRectF(0.0, 0.0, size.width(), size.height()), 0.5);
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


