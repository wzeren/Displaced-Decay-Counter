#ifndef _LINE
#define _LINE

#include <exception>
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <math.h>
#include <vector>
#include "include/cornerpoint/vertex.h"

/* Line is supposed to be a 3-dimensional object, that represents an infinitely long line.
 * A line is defined/initiaded by two Vertices, that it goes through. 
 * One is chosen as the support vector 'S' and the other is used to calculate the slope of the line called Orientation 'O'.
 * The line can then mathematically represented by 'L(x)=S+x*O', where x can be out of (-inf,inf).
 */ 


class Line{

public:
	Line()=default;
	~Line() {};
	Line(Vertex,Vertex);

	//Returns both vertices that identify a line
	Vertex getOrientation(){return orientation;};
	Vertex getSupport(){return support;};

	Vertex getPointOnLine(double);				//Is basically the line equation f(x)
	static bool linesHit(Line,Line);			//Test to see, if two lines intersect in 3-dimensional space
	static Vertex linesHitPoint(Line,Line);			//Vertex of the point that two lines intersect
	
private:
	Vertex  orientation;					//direction of the line
	Vertex	support;					//one point on the line we use as kind of a base
};
























#endif
