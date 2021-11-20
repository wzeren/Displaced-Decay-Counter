#ifndef _CUBICDETECTOR
#define _CUBICDETECTOR


#include <exception>
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <math.h>
#include <vector>
#include "include/cornerpoint/vertex.h"
#include "include/cornerpoint/plain.h"
#include "include/cornerpoint/line.h"


/* This class represents our cubic detector. It takes the 'front' and the 'back' polygonal plain corner points of the detector as input. They should roughly be parallel to each other
 * These are given as sides 0 and 1. For mAPP these are rectangles, but generally they can be any (even irregular) polygons, as long as both
 * back and front have the same shape. The other sides are then calculated from respectively 2 vertices of the front- and back-plain.
 * 
 * The corners need to be given in a coordinate system in which the interaction point is the origin (0,0,0)!
 *
 * Using the Plain-class we can determine here, if a detector is hit by an event, where it is hit, where the event leaves the detector and the respective lengths
 * we need for calculating the decay probability.
 *
 */


class CubicDetector{
public:

	CubicDetector(std::vector<Vertex>,std::vector<Vertex>,int);
	CubicDetector()=default;
 	~CubicDetector() { };

 	//Returning individual properties of the detector
	Plain getSide(int i){return sides[i];};
	Vertex getCorner(int i){return corners[i];};
	Vertex getCentOfDet(){return centerOfDetector;};
	int getHitIn(){return hitPlainIn;};
	int getHitOut(){return hitPlainOut;};


	bool isDetectorHit(Line);		//Determining if the detector is hit. This also stores, which plane is hit for the event going in, and which plane it is going 							//out as internal parameters
	double lengthToDetector(Line);		//Calculating the length of an event until it hits the detector
	double lengthInDetector(Line);		//Calculating the length an event travels in the detector
	
private:
	std::vector<Plain> sides;		//Sides of a detector. For a n-polygonal base shape, we have n+2 sides
	std::vector<Vertex> corners; 		//Corners of a detector. For a n-polygonal base shape, we have 2*n corners. the corners from 0 to n-1 are the 'front' corners 							//entered, the others are the back corners
	Vertex centerOfDetector;		//Center of the detector
	int amountOfSideCorners,amountOfSides;	//2*n and n+2, for a n-polygonal base shape
	int hitPlainIn,hitPlainOut;		//Integers that store, which plain is hit going in / out of the detector
};





















#endif
