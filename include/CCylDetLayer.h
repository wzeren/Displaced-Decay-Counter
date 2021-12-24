#ifndef _CCYLDETLAYER_
#define _CCYLDETLAYER_

#include <vector>
#include "include/CCylSeg.h"
#include <iostream>
#include <fstream>


 // The class CylDetLayer defines a detector layer in cylindrical coordinates.
 //  It consists of a list (vector) of oriented cylindrical segments,
 //  together with a weighing factor, primarily representing the angular aperture
 //  around the cylindrical axis (Dphi).
 //  An average sensitivity to the decay products could also be included.

class CylDetLayer{
    std::vector<CylSeg> CylSegList;
    double weight;
  public:
    CylDetLayer(std::vector<CylSeg>,double);
    CylDetLayer(std::vector<std::array<double,2>>,double);
    double inDetDec(double,double);
};
#endif
