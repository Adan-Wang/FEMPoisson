#pragma once
#include"shapes.h"
#include<array>
#include<random>
#include<algorithm>
//Implements the outer bounds of the simulation domain, center of the bounding box is defined as (x,y)=(0,0)

class boundingbox {
public:
	boundingbox(double length, double height);

	void addShape(shape* shape, bool inside);
	std::vector<std::array<double,2>> generateUniformGrid2DRand(double spacing, double chance);
	std::vector<double> obtainLW();


private:
	double mLength;
	double mWidth;
	double nXBound;
	double pXBound;
	double nYBound;
	double pYBound;
	std::vector<shape*> mComponents; //Components is a vector of pointers to the shape abstract class, it points to concrete derived classes from this base class
	std::vector<bool> mComponentInclusion;

};