#include <iostream>
#include<vector>
#include"boundingbox.h"
#include "delaunayBW.h"
#include<fstream>
#include<map>
#include<chrono>

int main() {

	std::ofstream grid;
	std::ofstream triangles;

	double domainHeight = 10e-6;
	double domainLength = 10e-6;
	double uniformSpacing = 0.01e-6;

	boundingbox domain(domainHeight, domainLength);
	std::vector<double> circleCenter(2, 0);
	circle* circle1 = new circle(circleCenter, 5e-6);
	domain.addShape(circle1, true);
	//circle* circle2 = new circle(circleCenter, 2.5e-6);
	//domain.addShape(circle2, false);

	std::vector<double> squareCenter(2, 0);
	rectangle* square1 = new rectangle(squareCenter, 5e-6, 4e-6);
	domain.addShape(square1, false);

	auto gridStart =  std::chrono::high_resolution_clock::now();
	std::vector < std::array<double, 2>> gridPoints = domain.generateUniformGrid2DRand(uniformSpacing, 0.6);

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
	auto gridStop = std::chrono::high_resolution_clock::now();
	auto gridTime = std::chrono::duration_cast<std::chrono::milliseconds> (gridStop - gridStart);

	std::cout.precision(4);
	std::cout << "Grid generation complete, a total of " << gridPoints.size() << " points generated in " << gridTime.count() << " milliseconds" << std::endl;


	auto triangulationStart = std::chrono::high_resolution_clock::now();
	BWmesh* mesh = new BWmesh();
	std::vector<BWtriangle> triangleMesh = mesh->generateBWDelaunay(domain, gridPoints);
	auto triangulationStop = std::chrono::high_resolution_clock::now();
	auto triangulationTime = std::chrono::duration_cast<std::chrono::seconds> (triangulationStop - triangulationStart);

	std::cout.precision(4);
	std::cout << "Triangulation complete, a total of " << triangleMesh.size() <<" triangles generated in " << triangulationTime.count() << " seconds" << std::endl;

	triangles.open("triangles.csv");
	for (auto triangle : triangleMesh) {
		std::array<BWvertex,3> vertexArray = triangle.getVertex(); //get all vertex
		for (auto vertex : vertexArray) {
			std::array<double, 2> vertexCoords = vertex.getCoord();
			triangles << gridLookUp[vertexCoords[0]][vertexCoords[1]] << ",";
		}
		triangles << std::endl;
	}
	

	std::cout << "Triangles Saved in csv files" << std::endl;
	system("py visualizeTriplot.py");

	return 0;
}