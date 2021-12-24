// class Detector defining a full detector
#ifndef _CCDetector_
#define _CCDetector_

#include <string>
#include "include/CCylDetLayer.h"

/* The detector class collects together a list (vector) of cylindrical detector layers.
    It is labelled with an identifier and receives a default integrated luminosity.
    Both of these properties can be read, as well as the detector's acceptance.
   This is the 'macroscopic' object that is tested in the simulation. */

class Detector{
    std::string Detname;                  // detector identifier
    double DetLumi;                       // detector luminosity
    std::vector<CylDetLayer> CylLayList;  // built out of a list of detector layers
  public:
    Detector(std::string,double,std::vector<CylDetLayer>);   // straightforward constructor
    double DetAcc(double,double);         // weighed decay probability within the detector layer
    std::string readname();                    // reads the detector's name
    double readLumi();                         // reads the detector's luminosity
};

/* Auxiliary function to build detector layers. */

CylDetLayer CylBrick(std::array<double,2> coord, double length, double height, double apphi, double wgh);

#endif
