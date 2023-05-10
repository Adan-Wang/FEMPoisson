#pragma once
#include<vector>
#include<array>
#include"shapes.h"
#include"boundingbox.h"
#include<functional>
#include<unordered_map>

class BWvertex {
public:
	BWvertex(double x, double y); //ctor
	std::array<double, 2> getCoord() const; //get coordinates

	bool operator == (const BWvertex b) const {
		return (mX == b.mX && mY == b.mY);
	}


private:
	double mX;
	double mY;
	

};

class BWedge {
public:
	BWedge(BWvertex v1, BWvertex v2);
	std::array<BWvertex, 2> getVertex() const;
	std::array<double, 4> getCoord() const; //Gets coordinates of the edges in 4 doubles, x1 y1, x2 y2
	bool operator == (const BWedge b) const {
		return (mV1 == b.mV1 && mV2 == b.mV2);
	}

private:
	BWvertex mV1;
	BWvertex mV2;

};

class BWtriangle {
public:
	BWtriangle(BWvertex v1, BWvertex v2, BWvertex v3);
	bool inCircumCirc(BWvertex vj);
	std::array<BWvertex, 3> getVertex() const; //get coordinates;
	bool shareVertex(BWtriangle t); //check if two triangles share a vertex
private:
	BWvertex mV1;
	BWvertex mV2;
	BWvertex mV3;
	circle mCircumCirc;

};

class BWmesh {
public:
	BWmesh();
	std::vector<BWtriangle> generateBWDelaunay(boundingbox domain, std::vector < std::array<double, 2>> gridpoints);

};

//Aux functions
std::vector<BWtriangle> addVertexAndCheck(std::array<double, 2> point, std::vector<BWtriangle> triangleArray);
std::vector<BWedge> findUniqueEdges(std::vector <BWedge>);


//Hash function for edge to do unique edge via hashmap --I am not sure if this is a good hash function at all...
namespace std {
	template<>
	struct std::hash<BWedge>
	{
		std::size_t operator()(const BWedge& edge) const {
			std::array<double, 4> edgeCoords = edge.getCoord();
			return (std::hash<double>()(edgeCoords[0]) ^ hash<double>()(edgeCoords[1]) << 1) >> 1 ^ hash<double>()(edgeCoords[2]) << 1 ^ hash<double>()(edgeCoords[2]) << 1;
		}

	};
}