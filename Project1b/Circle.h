#pragma once

class Circle
{
public:
	Circle();
	Circle(double x, double y, double radius, double r, double g, double b);

	double getX() { return mX; }
	double getY() { return mY; }
	double getRadius() { return mRadius; }
	double getR() { return mR; };
	double getG() { return mG; };
	double getB() { return mB; };
	double getDx() { return mXDir; }
	double getDy() { return mYDir; }
	double getNextX() { return mX + mXDir; }
	double getNextY() { return mY + mYDir; }

	void setDx(double dx) { mXDir = dx; }
	void setDy(double dy) { mYDir = dy; }
	void setX(double x) { mX = x;  }
	void setY(double y) { mY = y; }

	void Step(int myself, Circle all[], int count);
	void updatePosition(double G, double screen_x, double screen_y);
private:
	double mX, mY, mRadius;
	double mXDir, mYDir;
	double mR, mG, mB;
};
