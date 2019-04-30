#include "UnitTestGrid.h"
#include "Grid.h"
#include "Bodies\FluidParticle.h"
#include <QRectF>

class testBody : public  Body {
	void draw() {

	}
};

UnitTestGrid::UnitTestGrid()
{
}


UnitTestGrid::~UnitTestGrid()
{
}

void UnitTestGrid::run() {
	auto grid = Grid(QSize(24, 18), 3);
	auto body1 = new testBody();
	body1->boundingRect = QRectF(1, 1, 4, 5);
	auto body2 = new testBody();
	body1->boundingRect = QRectF(12, 10, 1, 2);

	auto body3 = new testBody();
	body1->boundingRect = QRectF(4, 2.9, 2.2, 1.2);

	BodiesVector bodies = { body1, body2,  body3 };
	grid.addBodiesToGrid(bodies);
	printf("tests valid");
}
