// class Detector defining a full detector
#ifndef _CCDetector_
#define _CCDetector_

#include <vector>
#include "include/CCylSeg.h"
#include "include/CCylDetLayer.h"


class Detector{
    std::vector<CylDetLayer> CylLayList;  // built out of a list of detector layers
  public:
    Detector(std::vector<CylDetLayer>);   // straightforward constructor
    double DetAcc(double,double);         // weighed decay probability within the detector layer
};

#endif
