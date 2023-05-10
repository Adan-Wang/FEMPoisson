#include <iostream>
#include<vector>
#include"boundingbox.h"
#include "delaunayBW.h"
#include<fstream>
#include<map>

int main() {

	std::ofstream grid;
	std::ofstream triangles;

	double domainHeight = 10e-6;
	double domainLength = 10e-6;
	double uniformSpacing = 1e-7;

	boundingbox domain(domainHeight, domainLength);
	std::vector<double> circleCenter(2, 0);
	circleCenter[0] = 0;
	circleCenter[1] = 0;
	circle* circle1 = new circle(circleCenter, 5e-6);
	domain.addShape(circle1, true);
	circle* circle2 = new circle(circleCenter, 2.5e-6);
	domain.addShape(circle2, false);


	std::vector < std::array<double, 2>> gridPoints = domain.generateUniformGrid2DRand(uniformSpacing, 0.5);

	std::map<double,std::map<double, int>> gridLookUp;

	for (int i = 0; i < gridPoints.size(); ++i) {
		std::array<double, 2> currentPoint = gridPoints[i];
		gridLookUp[currentPoint[0]][currentPoint[1]] = i; //create coordinate to index table
	}

	grid.open("grid.csv");
	for (auto pt : gridPoints) {
		grid << pt[0] << "," << pt[1] << std::endl;
	}
	grid.close();
	
	BWmesh* mesh = new BWmesh();
	std::vector<BWtriangle> triangleMesh = mesh->generateBWDelaunay(domain, gridPoints);
	triangles.open("triangles.csv");
	for (auto triangle : triangleMesh) {
		std::array<BWvertex,3> vertexArray = triangle.getVertex(); //get all vertex
		for (auto vertex : vertexArray) {
			std::array<double, 2> vertexCoords = vertex.getCoord();
			triangles << gridLookUp[vertexCoords[0]][vertexCoords[1]] << ",";
		}
		triangles << std::endl;
	}

	std::cout << "Hi" << std::endl;

	return 0;
}