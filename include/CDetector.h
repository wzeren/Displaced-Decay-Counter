// class Detector defining a full detector
#ifndef _CCDetector_
#define _CCDetector_

#include <string>
#include "include/CCylDetLayer.h"


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

CylDetLayer CylBrick(std::array<double,2> coord, double length, double height, double apphi, double wgh);

#endif
