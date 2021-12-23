// CylSeg class defining oriented segments in the cylindrical plane {z,y}
#ifndef _CCYLSEG_
#define _CCYLSEG_

#include <array>

class CylSeg{
    double zA,yA,zB,yB,zeqn,yeqn,detv;
  // {zA,yA} and {zB,yB} are the coordinates of the extreme points
  // detv=zA*yB-zB*yA: detv=0 corresponds to an uninteresting segment,
  //   either trivial or aligned with the interaction point
  // points {z,y} in the segment satisfy the equation zeqn*z+yeqn*y=1.
  //   the case zeqn*z+yeqn*y=0. is irrelevant (aligned with IP)
    int flysign;
  // sign determining the orientation of the segment
  public:
    CylSeg(std::array<double,2>,std::array<double,2>,int);
  // constructor from pointers to the coordinates of the extreme
  //   points and the orientation
    double DecProb(double,double);
  // function computing the exponential factor from the angle of  
  //   emission of the particle and effective flying distance
};


#endif
