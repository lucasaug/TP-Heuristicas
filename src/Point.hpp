#ifndef TI_POINT
#define TI_POINT

/*
 * Simple point class
 * Represents a 2D point coordinate and calculates
 * distances between two points
 */

class Point {
	private:
	float x, y;

	public:
	Point(float, float);
	float getX();
	float getY();

	int EUC_2D_distance(Point);
	int ATT_distance(Point);
};

#endif
