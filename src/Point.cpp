#include <cmath>

#include "Point.hpp"

Point::Point(float nx, float ny) {
	x = nx;
	y = ny;
}

float Point::getX() {
	return x;
}

float Point::getY() {
	return y;
}

int Point::EUC_2D_distance(Point p) {
	int xd = x - p.x,
	    yd = y - p.y;

	return round(std::sqrt(xd * xd + yd * yd));
}

int Point::ATT_distance(Point p) {
	int xd = x - p.x,
	    yd = y - p.y;

	return std::ceil(std::sqrt((xd * xd + yd * yd) / 10.0));
}
