#include "shapes.h"

shape::shape(std::vector<double> center) {
	shape::mCenter = center;
}


rectangle::rectangle(std::vector<double> center, double width, double height) : shape(center) {
	mWidth = width;
	mHeight = height;
	mCenterX = center[0];
	mCenterY = center[1];
}

bool rectangle::withinBounds(double x, double y) {
	if (x > mCenterX - 0.5 * mWidth && x < mCenterX + 0.5 * mWidth && y> mCenterY - 0.5*mHeight && y<mCenterY+0.5*mHeight ) {
		return true;
	}
	return false;
}

circle::circle(std::vector<double>center, double radius) : shape(center) {
	mRadius = radius;
	mCenterX = center[0];
	mCenterY = center[1];
}

bool circle::withinBounds(double x, double y) {
	if (((x - mCenterX) * (x - mCenterX) + (y - mCenterY) * (y - mCenterY)) < (mRadius*mRadius)) {
		return true;
	}
	return false;
}