#ifndef _CCYLDETLAYER_
#define _CCYLDETLAYER_

#include <array>
#include <vector>
#include "include/CCylSeg.h"
#include <math.h>
#include <iostream>
#include <fstream>


// class CylDetLayer defining a detector layer in cylindrical coordinates
class CylDetLayer{
    std::vector<CylSeg> CylSegList;   // built out of a list of oriented cylindrical segment
    double weight;                    // weighing factor: phi-opening, luminosity, 
                                      //                  efficiency for detection of the LLP decay products
  public:
    CylDetLayer(std::vector<CylSeg>,double);  // straightforward constructor
    CylDetLayer(std::vector<std::array<double,2>>,double); // constructor from a list of coordinates in the {z,y} plane
    double inDetDec(double,double);           // weighed decay probability within the detector layer
};
#endif
