#include"delaunayBW.h"
#include "shapes.h"

BWvertex::BWvertex(double x, double y): mX(x), mY(y) {
	
}



std::array<double,2> BWvertex::getCoord() const { //Coordinate getter
	std::array<double, 2> out = { mX, mY };
	return out;
}

BWedge::BWedge(BWvertex v1, BWvertex v2): mV1(v1), mV2(v2) {
	
}

std::array<double, 4> BWedge::getCoord() const {
	std::array<double, 2> v1Coord = mV1.getCoord();
	std::array<double, 2> v2Coord = mV2.getCoord();
	return std::array<double, 4>{v1Coord[0], v1Coord[1], v2Coord[0], v2Coord[1]};
}

std::array<BWvertex, 2> BWedge::getVertex() const {
	return std::array<BWvertex, 2>{mV1, mV2};
}

BWtriangle::BWtriangle(BWvertex v1, BWvertex v2, BWvertex v3) : mV1(v1), mV2(v2),mV3(v3) {
	auto distance = [](BWvertex v1, BWvertex v2) -> double { //euclidean distance subroutine, look up lambda functions if confused
		std::array<double, 2> v1Coord = v1.getCoord();
		std::array<double, 2> v2Coord = v2.getCoord();
		return sqrt((v2Coord[0]- v1Coord[0]) * (v2Coord[0] - v1Coord[0]) + (v2Coord[1] - v1Coord[1]) * (v2Coord[1] - v1Coord[1]));
	};

	//Find radius of circumcircle
	double a = distance(v1, v2);
	double b = distance(v1, v3);
	double c = distance(v2, v3);
	double R = (a * b * c) / (sqrt((a + b + c) * (-a + b + c) * (a - b + c) * (a + b - c)));

	//Find center of circle
	std::array<double, 2> v1Coord = v1.getCoord();
	std::array<double, 2> v2Coord = v2.getCoord();
	std::array<double, 2> v3Coord = v3.getCoord();

	//Re-cast array elements into references to make calling them easier, use reference to save memory
	double& x1 = v1Coord[0];
	double& y1 = v1Coord[1];

	double& x2 = v2Coord[0];
	double& y2 = v2Coord[1];

	double& x3 = v3Coord[0];
	double& y3 = v3Coord[1];

	double centerDenom = 2 * (x1 - x2) * (y1 - y3) - 2 * (x1 - x3) * (y1 - y2);
	double centerX = ((x1 * x1 + y1 * y1 - x2 * x2 - y2 * y2) * (y1 - y3) - (x1 * x1 + y1 * y1 - x3 * x3 - y3 * y3) * (y1 - y2)) / centerDenom;
	double centerY = ((x1 - x2) * (x1 * x1 + y1 * y1 - x3 * x3 - y3 * y3) - (x1 - x3) * (x1 * x1 + y1 * y1 - x2 * x2 - y2 * y2)) / centerDenom;

	std::vector<double> center = { centerX, centerY };

	mCircumCirc=circle::circle(center, R);
}

bool BWtriangle::inCircumCirc(BWvertex vj) {
	std::array<double, 2> vjCoord = vj.getCoord();
	
	return (mCircumCirc.withinBounds(vjCoord[0], vjCoord[1]));

}

std::array<BWvertex, 3> BWtriangle::getVertex() const { //Coordinate getter, outputs vertexes
	std::array<BWvertex, 3> out = {mV1, mV2, mV3 };
	return out;
}

bool BWtriangle::shareVertex(BWtriangle t) {
	return (mV1 == t.mV1 || mV1 == t.mV2 || mV1 == t.mV3 || mV2 == t.mV1 || mV2 == t.mV2 || mV2 == t.mV3 || mV3 == t.mV1 || mV3 == t.mV2 || mV3 == t.mV3);
}



BWmesh::BWmesh() {//empty ctor for mesh for now

}

//Aux function to generate mesh, can probably implement with lambda but it might be a bad idea because then we might need to embed lambdas in lambdas (not sure about function scoping..)
std::vector<BWtriangle> addVertexAndCheck(std::array<double, 2> point, std::vector<BWtriangle> triangleArray) {
	//intialize edge aray for later
	std::vector<BWedge> edges;
	std::vector<BWtriangle> tOutputTriangles;
	BWvertex ptVertex = BWvertex(point[0], point[1]);
	//check all triangles with this vertex, is this vertex in any triangles, if so, remove that triangle
	for (BWtriangle triangle : triangleArray) {
		
		if (triangle.inCircumCirc(ptVertex)) {
			std::array<BWvertex, 3> tEdge = triangle.getVertex();
			//push back all the edges
			edges.push_back(BWedge(tEdge[0], tEdge[1]));
			edges.push_back(BWedge(tEdge[0], tEdge[2]));
			edges.push_back(BWedge(tEdge[1], tEdge[2]));
		}
		else {
			tOutputTriangles.push_back(triangle);
		}//if point is not in any of the triangles in array, triangle is valid
	}

	
	//Find the set of unique edges from the edge collection, this is n^2 in edge collection in the worst case,
	//TO-DO: look into this and fix it up
	// I implemented somethnig hacky (without knowing how the std::hashing function actually works or why we need to bitshift...etc) for now
	edges = findUniqueEdges(edges); //Overwrite edges with unique edges

	//Create new triangles from unique edge list using the vertex
	for (BWedge oneEdge : edges) {

		std::array<BWvertex, 2> edgeVertecies= oneEdge.getVertex();
		BWtriangle tempTriangle = BWtriangle(edgeVertecies[0], edgeVertecies[1], ptVertex);
		tOutputTriangles.push_back(tempTriangle);
	}
	return tOutputTriangles;
}

std::vector<BWedge> findUniqueEdges(std::vector <BWedge> inputVector) {

	std::unordered_map<BWedge, int> uniqueEdgeMap;
	std::vector<BWedge> uniqueEdges;

	for (int i = 0; i < inputVector.size(); ++i) {
		uniqueEdgeMap[inputVector[i]]++;
	}

	for (const auto& kv : uniqueEdgeMap) {
		if (kv.second == 1) {
			uniqueEdges.push_back(kv.first);
		}
	}

	return uniqueEdges;



}



std::vector<BWtriangle> BWmesh::generateBWDelaunay(boundingbox domain, std::vector < std::array<double, 2>> gridpoints) {
	//Procedure (https://www.gorillasun.de/blog/bowyer-watson-algorithm-for-delaunay-triangulation/):

	//Create super triangle, add to array
	//Iterate over all points
	//A. Check if any points are within the circumcircle of any of the triangles in the array of triangles
	//B. Add point to form more triangles
	//Step A, if there are any points in any of the triangles, those triangles are wrong and deleted
	//Step B
	//Remove all triangles that share edge with the initial super triangle
	// 
	// 
	// 
	// 
	
	//Initialization
	//TO-DO: look into the aux array issue
	std::vector<BWtriangle>tempTriangles; //We'll use auxiliary array for now, can probably use reference and stl delete but it might be risky doing that while iterating
	std::vector<BWtriangle>outputTriangles;


	//Create supertriangle, assume rectangular domain and make a triangle that encompasses the domain as if it was a square
	//This is a large triangle right now, it may be worth fixing in the future
	std::vector<double> domainSize = domain.obtainLW();
	double longSide;
	if(domainSize[0]>domainSize[1]){
		longSide = domainSize[0];
	}
	else {
		longSide = domainSize[1];
	}

	double longSidePrime = longSide * 1.1; //make triangle slightly larger than even the square to avoid edge issues
	
	BWtriangle superTriangle = BWtriangle(BWvertex(-longSidePrime / 2, -longSidePrime / 2), BWvertex(1.5 * longSidePrime, -longSidePrime / 2), BWvertex(-longSidePrime / 2, 1.5 * longSidePrime));

	tempTriangles.push_back(superTriangle);
	
	//Iterate over all points

	for (auto point : gridpoints) {
		tempTriangles = addVertexAndCheck(point, tempTriangles); //Add each vertex and eliminate invalid triangles
	}

	for (auto triangle : tempTriangles) {
		if (!triangle.shareVertex(superTriangle)) {
			outputTriangles.push_back(triangle);
		}
	}

	return outputTriangles;

}