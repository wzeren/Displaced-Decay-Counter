#ifndef _VERTEX
#define _VERTEX

#include <exception>
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <math.h>
#include <vector>
#include "include/cornerpoint/matrix.h"


#define MY_PI acos(-1.0) // most accurate definition of Pi in C++


/* This class is part of the mathematical framework of Detectors. 
 * It represents a point in the 3-dimensional space.
 * There are also some basic mathematical operations between vertices and rotation matrices implemented
 */

class Vertex {
public:
 	Vertex(double,double,double,std::string);
	Vertex()=default;
 	~Vertex() { };
 	void update (double,double,double,std::string);			//update of a vertice, maybe not really necessary

  	//Return of entries of a Vertex in all 3 Coordinate Systems. Array is implemented for "for-loops". Definiteness keeps track of the angles
  	double getX(){return vk_x;};
 	double getY(){return vk_y;};
 	double getZ(){return vk_z;};
 	double getR(){return vp_r;};
 	double getPhi(){return vp_phi;};
 	double getTheta(){return vp_theta;};
 	double getCylR(){return vc_r;};
 	double getCylPhi(){return vc_phi;};
 	double getCylZ(){return vc_z;};
 	double getVecArray(int i){return vecArray[i];};
 	bool check_definiteness(){return def;};


 	static Vertex addVertices(Vertex,Vertex);			//Addition of Vertices
 	static Vertex subtractVertices(Vertex,Vertex);			//Subtraction of Vertices
 	static double vertexNorm(Vertex);				//Length of a Vertex to the Origin
 	static double scalarProduct(Vertex,Vertex); 			//Scalar product of two Vertices
 	static double vertexLength(Vertex);				//Length of a Vertex
 	static Vertex crossProduct(Vertex,Vertex);			//Cross product of two vertices
 	static Vertex multMatrixVertex(Vertex,Matrix);			//Multiplikation of a Matrix with a Vertex
 	bool isNull();							//checking, if the vertex is the null vertex/origin
	static void readOutVecCart(Vertex);
	static void readOutVecPol(Vertex);
  
private:
  	double vk_x,vk_y,vk_z,vp_r,vp_phi,vp_theta,vc_r,vc_phi,vc_z;	//coordinates of vertice in all coordinate system
  	double vecArray[3];						//array for easier handling of vertices in for loops, given in cartesian coordinates
  	bool def=true;							//boolean to check if angles are properly defined
};






#endif
