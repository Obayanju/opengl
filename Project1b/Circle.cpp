#include "Circle.h"

Circle::Circle() {}

Circle::Circle(double x, double y, double radius, double r, double g, double b)
	: mX(x), mY(y), mRadius(radius), mR(r), mG(g), mB(b) {
}

void Circle::updatePosition(double G, double screen_x, double screen_y) {
	double x = this->getX();
	double dX = this->getDx();
	double y = this->getY();
	double dY = this->getDy();
	double radius = this->getRadius();

	if (x + dX + radius >= screen_x)
		this->setDx(-dX);
	if (x - radius + dX < 0)
		this->setDx(-dX);
	if (y + dY + radius >= screen_y)
		this->setDy(-dY);
	if (y - radius + dY < 0)
		this->setDy(-dY);

	this->setDy(this->getDy() - G);
	this->setY(y + this->getDy());
	this->setX(x + this->getDx());
}
