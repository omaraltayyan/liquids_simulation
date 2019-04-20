#pragma once
#include <iostream>
#include <boost/geometry.hpp>

namespace bg = boost::geometry;
class Body
{
public:

	virtual void draw() = 0;

	typedef bg::model::point<double, 2, bg::cs::cartesian> Point;
	typedef bg::model::box<Point> Rectangle;
	Rectangle boundingRect;


	Body();
	~Body();
};

