#include "FluidParticle.h"
#include "PhysicsEngine.h"
#include <qmath.h>

FluidParticle::FluidParticle(const QPointF& position, PhysicsEngine* engine, qreal sizeRadius,
	double viscosity, double mass, double gasConstant, double restDesity,
	double surfaceTension, double threshold) : Particle(position, engine, sizeRadius)
{
	this->radius_2 = -100.0;
	this->radius_4 = -100.0;
	_viscosity = viscosity;
	_mass = mass;
	_gasConstant = gasConstant;
	_restDensity = restDesity;
	_velocity.setX(0.0);
	_velocity.setY(0.0);

	_accelration.setX(0.0);
	_accelration.setY(0.0);

	_velocityHalfStep.setX(0);
	_velocityHalfStep.setY(0);

	_tensionCoefcioant = surfaceTension;
	_surfaceThreshold = threshold;
	_isFirstIteration = true;
	this->bodyType = fluid;
}


double FluidParticle::applyKernal(double distance, double radius, SmoothingKernals kernal)
{
	if (radius_2 < 0.0)
	{
		radius_2 = radius * radius;
		radius_4 = radius_2 * radius_2;
	}

	double holder = radius_2 - (distance * distance);
	switch (kernal)
	{
	case poly6:
		return (315 / (64 * M_PI*radius_4*radius_4*radius)) * (holder*holder*holder);
	case gradPoly6:
		return -1 * (945 / (32 * M_PI * radius_4*radius_4*radius)) * (holder*holder);
	case lapPoly6:
		return ((-1 * 945) / (32 * M_PI * radius_4*radius_4*radius)) * holder * ((3 * radius*radius) - (7 * distance*distance));
	case spiky:
		return (-45 / (radius_4*radius_2)) * ((radius - distance) * (radius - distance));
	case visc:
		return (45 / (radius_4*radius_2)) * (radius - distance);
	default:
		return 0.0;
	}


}

double FluidParticle::computeDensity(const QVector<FluidParticle*>& fuildParticles, double radius)
{
	double resultingDynsity = 0.0;
	for (int i = 0; i < fuildParticles.length(); i++)
	{
		auto particle = fuildParticles[i];
		double distance = this->positionVector.distanceToPoint(particle->positionVector);
		resultingDynsity += particle->_mass * this->applyKernal(distance, radius, poly6);
	}
	return resultingDynsity;
}

double FluidParticle::computePressure(double gasConstant, double restDynsity, double density)
{
	//if (density < restDynsity) {
	//	return 0.01 * gasConstant * (density - restDynsity);
	//}
	return gasConstant  * (density - restDynsity);
}

QVector2D FluidParticle::computePressureForce(const QVector<FluidParticle*>& fuildParticles, double radius)
{
	QVector2D resultingPressureForce(0.0, 0.0);
	auto relativePressureTerm = this->_pressure / (this->_density * this->_density);
	for (int i = 0; i < fuildParticles.length(); i++)
	{
		auto particle = fuildParticles[i];

		if (particle == this)
			continue;
		auto vec = this->positionVector - particle->positionVector;
		vec.normalize();
		double distance = this->positionVector.distanceToPoint(particle->positionVector);

		auto leftTerm = (relativePressureTerm + (particle->_pressure / (particle->_density * particle->_density))) * particle->_mass;
		resultingPressureForce += vec * leftTerm * this->applyKernal(distance, radius, spiky);
	}
	return (-this->_density) *  resultingPressureForce;
}

QVector2D FluidParticle::computeViscousForce(const QVector<FluidParticle*>& fuildParticles, double radius)
{
	QVector2D resultingVisousForce(0, 0);
	for (int i = 0; i < fuildParticles.length(); i++)
	{
		auto particle = fuildParticles[i];

		if (particle == this)
			continue;
		double distance = this->positionVector.distanceToPoint(particle->positionVector);
		resultingVisousForce += particle->_mass * (((particle->_velocity) - (this->_velocity)) / particle->_density)
			* this->applyKernal(distance, radius, visc);
	}

	return this->_viscosity * resultingVisousForce;
}

//important note : 
//when useLaplacian equlas false this method will compute the surface normal (whiche is the gradient to the color feild)
//when useLaplaican equals true this method wii compute the laplacian of the color feild to use it to compute the surfacetension
QVector2D FluidParticle::computeSurfaceNormal(const QVector<FluidParticle*>& fuildParticles, double radius)
{
	QVector2D resultingSurfaceNormal(0.0, 0.0);
	for (int i = 0; i < fuildParticles.length(); i++)
	{
		auto particle = fuildParticles[i];
		if (particle == this)
			continue;
		auto vec = this->positionVector - particle->positionVector;
		double distance = this->positionVector.distanceToPoint(particle->positionVector);
		double kernel = this->applyKernal(distance, radius, gradPoly6);
		resultingSurfaceNormal += vec * (particle->_mass / particle->_density)*kernel;
	}
	return resultingSurfaceNormal;
}


//important note : 
//when useLaplacian equlas false this method will compute the surface normal (whiche is the gradient to the color feild)
//when useLaplaican equals true this method wii compute the laplacian of the color feild to use it to compute the surfacetension
double FluidParticle::computeLaplacianSurfaceNormal(const QVector<FluidParticle*>& fuildParticles, double radius)
{
	double resultingSurfaceNormal = 0.0;
	for (int i = 0; i < fuildParticles.length(); i++)
	{
		auto particle = fuildParticles[i];
		if (particle == this)
			continue;
		auto vec = this->positionVector - particle->positionVector;
		double distance = this->positionVector.distanceToPoint(particle->positionVector);
		double kernel = this->applyKernal(distance, radius, lapPoly6);
		resultingSurfaceNormal += (particle->_mass / particle->_density)*kernel;
	}
	return resultingSurfaceNormal;
}

QVector2D FluidParticle::computeSurfaceTension(const QVector<FluidParticle*>& fuildParticles, double radius)
{
	QVector2D resultingSurfaceTension(0.0, 0.0);
	auto surfaceNormal = this->computeSurfaceNormal(fuildParticles, radius);
	double magnitude = surfaceNormal.length();
	if (magnitude >= this->_surfaceThreshold)
	{
		surfaceNormal.normalize();
		resultingSurfaceTension = this->computeLaplacianSurfaceNormal(fuildParticles, radius) * surfaceNormal;
	}

	return -1 * this->_tensionCoefcioant * resultingSurfaceTension;
}

QVector<FluidParticle*> FluidParticle::filterFuildParticles(const QVector<BodiesVector*>& surroundingBodies, double radius)
{
	int bodiesCount = 0;
	for (int i = 0; i < surroundingBodies.length(); i++)
	{
		bodiesCount += surroundingBodies[i]->length();
	}
	QVector<FluidParticle*> fluidParticles;
	fluidParticles.reserve(bodiesCount);

	for (int i = 0; i < surroundingBodies.length(); i++)
	{
		auto bodyVector = surroundingBodies[i];
		for (int j = 0; j < bodyVector->length(); j++)
		{
			auto body = bodyVector->at(j);
			//FluidParticle* particle = dynamic_cast<FluidParticle*>(body);
			if (body->bodyType == fluid)
			{
				auto particle = (FluidParticle*)body;
				double distance = this->positionVector.distanceToPoint(particle->positionVector);
				if (distance <= radius) {
					fluidParticles.push_back(particle);
				}
			}
		}
	}
	return fluidParticles;
}

QVector2D FluidParticle::computeSumOfForces(const QVector<FluidParticle*>& fluidParticles, double radius)
{
	QVector2D pressureForce = this->computePressureForce(fluidParticles, radius);
	QVector2D viscousForce = this->computeViscousForce(fluidParticles, radius);
	QVector2D gravityForce =  this->engine->gravity * this->_density;
	QVector2D surfaceTensionForce = this->computeSurfaceTension(fluidParticles, radius);
	return pressureForce + viscousForce + gravityForce + surfaceTensionForce;
}

void FluidParticle::applyLeapFrogTimeStepIntegration()
{
	auto accelration = this->_force / this->_density;
	if (!this->_isFirstIteration)
	{
		
		this->_velocityHalfStep += engine->timeDelta * accelration;
		this->_velocity = this->_velocityHalfStep + (accelration * (engine->timeDelta / 2));
		
	}
	else
	{
		this->_velocityHalfStep = this->_velocity + (accelration * (engine->timeDelta / 2));
		this->_velocity += accelration * engine->timeDelta;
		this->_isFirstIteration = false;
	}
	this->positionVector += this->_velocityHalfStep * engine->timeDelta;
	this->setPosition(this->positionVector.toPointF());
	
}

void FluidParticle::detectCollision(double Xmin, double Xmax, double Ymin, double Ymax)
{
	QPointF centerPoint((Xmin+Xmax)/2,(Ymin+Ymax)/2);
	QPointF particleLocalPosition = this->position - centerPoint;
	QVector2D particleLocalPostionVector = QVector2D(particleLocalPosition);
	QVector2D extentVector = QVector2D(centerPoint);
	QVector2D evaluationVector = particleLocalPostionVector - extentVector;
	double evaluationfunction = qMax(evaluationVector.x(), evaluationVector.y());
	auto localContactPoint = qMin(extentVector, qMax(-extentVector, particleLocalPostionVector));
	auto contactPoint = extentVector + localContactPoint;
	auto penterationDepth = this->positionVector.distanceToPoint(contactPoint);
	QVector2D vec = localContactPoint - particleLocalPostionVector;
	vec.setX(this->signumFunction(vec.x()));
	vec.setY(this->signumFunction(vec.y()));
	auto normal = vec / vec.length();
	
	
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
	double radius = engine->getUnsafeBodiesGrid().squareSideInCentimeters();
	auto fluidParticles = this->filterFuildParticles(surroundingBodies, radius);

	if (calculationOperation == 0) {
		this->_density = this->computeDensity(fluidParticles, radius);
		//here gas constant and rest density should variables taken from user input
		this->_pressure = this->computePressure(this->_gasConstant, this->_restDensity, this->_density);
	}
	else if (calculationOperation == 1) {		
		//here gravity should also be taken from user input
		this->_force = this->computeSumOfForces(fluidParticles, radius);
	}
}

void FluidParticle::applyInteraction()
{
	
	this->applyLeapFrogTimeStepIntegration();
	auto size = this->engine->getUnsafeBodiesGrid().sizeInCentimeters();
	double damp = 0.25;

	if (this->position.x() < 0)
	{
		this->positionVector -= this->_velocity * (1 - damp) * (this->position.x() / this->_velocity.x());
		this->positionVector.setX(-this->positionVector.x());

		this->_velocity.setX(-this->_velocity.x());
		this->_velocity *= damp;

		this->_velocityHalfStep.setX(-this->_velocityHalfStep.x());
		this->_velocityHalfStep *= damp;

		this->setPosition(this->positionVector.toPointF());


	}

	if (this->position.x() > size.width())
	{
		this->positionVector -= this->_velocity * (1 - damp) * ((this->position.x() - size.width()) / this->_velocity.x());
		this->positionVector.setX((2 * size.width()) - this->positionVector.x());

		this->_velocity.setX(-this->_velocity.x());
		this->_velocity *= damp;

		this->_velocityHalfStep.setX(-this->_velocityHalfStep.x());
		this->_velocityHalfStep *= damp;

		this->setPosition(this->positionVector.toPointF());
	}
	if (this->position.y() < 0)
	{
		this->positionVector -= this->_velocity * (1 - damp) * ((this->position.y()) / this->_velocity.y());
		this->positionVector.setY(-this->positionVector.y());

		this->_velocity.setY(-this->_velocity.y());
		this->_velocity *= damp;

		this->_velocityHalfStep.setY(-this->_velocityHalfStep.y());
		this->_velocityHalfStep *= damp;

		this->setPosition(this->positionVector.toPointF());
	}
	if (this->position.y() > size.height())
	{
		this->positionVector -= this->_velocity * (1 - damp) * ((this->position.y() - size.height()) / this->_velocity.y());
		this->positionVector.setY((2 * size.height()) - this->positionVector.y());

		this->_velocity.setY(-this->_velocity.y());
		this->_velocity *= damp;

		this->_velocityHalfStep.setY(-this->_velocityHalfStep.y());
		this->_velocityHalfStep *= damp;

		this->setPosition(this->positionVector.toPointF());
	}	

}



