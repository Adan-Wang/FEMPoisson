#pragma once
#include<vector> //Use vector for center to extend to 3D, can probably use pair for 2D only simulations, or use array???
#include<string>
#include<iostream>

class shape { //Shape is an abstract class, if we want to pass it around we must pass by pointer and not by value
public:
	shape(std::vector<double> center);
	virtual bool withinBounds(double x, double y) = 0;

protected:
	std::vector<double>mCenter;

};

class circle:public shape {
public:
	circle(std::vector<double> center = {0,0}, double radius=0);
	bool withinBounds(double x, double y);

private:
	double mRadius;
	double mCenterX;
	double mCenterY;
};

class rectangle:public shape {
public:
	rectangle(std::vector<double> center, double width, double height);
	rectangle(const rectangle& p1);
	bool withinBounds(double x, double y);

private:
	double mHeight;
	double mWidth;
	double mCenterX;
	double mCenterY;
};