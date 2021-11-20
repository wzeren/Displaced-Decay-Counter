#ifndef _PLAIN
#define _PLAIN

#include <exception>
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <math.h>
#include <vector>
#include "vertex.h"
#include "matrix.h"
#include "include/cornerpoint/line.h"

/* This class is supposed to represent a plain in three dimensional space. The plain itself is going on infinitely,
 * but we also implement a 2-dimensional Polygonal shape onto the plain, as to represent a side of a detector.
 * For this, we need a number of vertices, that lay in the plain in a vector called Corners.
 *
 * To check if a line hits a specific area of the plain (i.e. a particle hits a wall of a detector) we rotate the plain to align with one of
 * the 3 axices, as it is easier to check this in a 2 dimensional case. Private Variables, that involve the word rotated or Rot
 * are dealing in this frame.
 */




class Plain {
public:
	Plain(const std::vector<Vertex> &, int);
	Plain()=default;
        ~Plain() { };


    	//Returning all the relevant Vertices and private Variables, important for intializing a plain
   	double getArea(){return area;};
    	int getNumberOfCorners(){return numberOfCorners;};
 	Vertex getNormalVector(){return normal;};
	Vertex getSupportVector(){return support;};
	Vertex getDirectionalVector1(){return direction1;};
	Vertex getDirectionalVector2(){return direction2;};
	Vertex getCenter(){return center;};
	Vertex getCorner(int i){return corners[i];};
	Vertex getRotCenter(){return rotatedCenter;};
	Vertex getRotCorner(int i){return rotatedCorners[i];};
	Line getRotEdge(int i){return rotEdges[i];};
	char getAlignmentAxis(){return alignmentAxis;};


	Vertex rotatedPlainVector(Vertex);		//Rotating a vector by the same angles as the plain, so we can work in the rotated frame
	Vertex rotatePlainVectorBack(Vertex);		//Undo the rotation of a vector
	Line rotatedLine(Line);				//Rotating a line by the same angles as the plain, so we can work in the rotated frame
	Line rotateLineBack(Line);			//Undo the rotation of a line


	bool hitPlain(Line);				//Checking if a vector hits the plain or is parallel to the plain
	Vertex hitRotPlainPoint(Line);			//The point, where a line hits the plain in the rotated frame
	Vertex hitPlainPoint(Line);			//The point, where a line hits the plain
	bool isAreaHit(Line);				//Checking if a vector hits the designated area

private:
  	Vertex normal,support,direction1, direction2;	//Vertices that characterise the infinite plain
  	double angle1,angle2;				//Rotation angles that are needed for the aligned frame
  	std::vector<Vertex> corners;			//Corners of the specific area
	std::vector<Vertex> rotatedCorners;		//Corners of the area in the rotated frame
	std::vector<Line> rotEdges;			//Edges of the specific area in the rotated frame, used for determining if the area is hit
  	int numberOfCorners;				//Number of corners
	double area;					//Area of the area, duh...
	Vertex center;					//Center of the Polygonal shape of the plain, need to check if the corners are labeled in the counter-clockwise way
	Vertex rotatedCenter;				//Center in the rotated frame
	char alignmentAxis;				//Axis, where we want to align the plain to. If it is equal to "E", there is an Error
	bool preAligned;				//Boolean to check, if the plain was already aligned (when rotating it)

	double calculateArea(int);			//Calculating the area, this is done in the rotated frame (2 dimensional)
	Vertex calculateCenter(int);			//Calculating the Center, this is done in the rotated frame (2 dimensional)
	void sortCorners();				//Algorithm to sort the corners
	bool isRight(Vertex,Vertex);			//Boolean to check, the relativ position of two vertices in the specific area in regards to a line through the center. 								//This happens in the rotated plane, it needs to be in 2 dimensions
	bool isSorted(std::vector<Vertex>);		//Checking if a vertex array is sorted counter-clockwise
	void calcAlignment();				//Obtaining, what the alignment needs to be
	void calcAlignmentAngles();			//Obtaining the angles we need, to rotate the plain and align it with a axis (typically the z-axis)
};






#endif






