#include"boundingbox.h"

boundingbox::boundingbox(double length, double height) {
	mLength = length;
	mWidth = height;
	nXBound = 0 - length / 2;
	nYBound = 0 - height / 2;
	pXBound = 0 + length / 2;
	pYBound = 0 + height / 2;
}

void boundingbox::addShape(shape* shape, bool inside) { //We use pointers to shapes becausee shape is a abstract class with no real definition to the virtual function, so we need to point to a concrete class
	//Inside=true means the domain will include the shape, otherwise it will exclude the shape, domain will ALWAYS be inside bounding box
	mComponents.push_back(shape);
	mComponentInclusion.push_back(inside);

}

std::vector<double> boundingbox::obtainLW() {
	return std::vector<double>{mLength, mWidth};
}

std::vector<std::array<double, 2>> boundingbox::generateUniformGrid2DRand(double spacing, double chance) { //Generate grid with a random chance of whether or not the point will be there
	//Generate points for all of the bounding box first, 100% chance so we get all the points
	std::vector < std::array<double, 2>> allPoints;
	std::default_random_engine gen;
	std::uniform_int_distribution<int> singleSideDist(0, 100);
	std::uniform_int_distribution<int> twoSideDist(-100, 100);
	
	for (double x = nXBound; x < pXBound; x = x + spacing) {
		for (double y = nXBound; y < pXBound; y = y + spacing) {
			//double xJitter = 0.1 * spacing * (double)twoSideDist(gen) / 100;
			//double yJitter = 0.5 * spacing * (double)twoSideDist(gen) / 100;
			std::array<double, 2> pt = {x,y};
			allPoints.push_back(pt);
		}
	}
	
	std::vector<std::array<double,2>> output;
	for (auto pt : allPoints) {
		int i = 0; //there's probably a cleaner way of doing this using one object or something, but that makes the shape part not seperate from the inside part
		bool keepPoint = true; //Assume we will keep the point
		for (auto component : mComponents) {
			if (mComponentInclusion[i]) {
				if (!component->withinBounds(pt[0], pt[1])) {
					keepPoint = false; //We don't keep the point if component is not within bounds, if the component is set to "include"
					break; //no point continuing if it doesnt satisfy one of the components
				}
			}
			else {
				if (component->withinBounds(pt[0], pt[1])) {
					keepPoint = false; //We don't keep the point if component *is* within bounds, if the component is set to "exclude"
					break; //no point continuing if it doesnt satisfy one of the components
				}
			}
			++i; //increment i
		}
		//After we go through all the components, we will know if we can keep the point, now we add in the RNG
		if (keepPoint && (singleSideDist(gen))<(chance*100)) {
			output.push_back(pt);
		}
	}

	//std::shuffle(std::begin(output), std::end(output), gen);

	return output;


};
