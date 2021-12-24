#ifndef _CCYLSEG_
#define _CCYLSEG_

#include <array>
#include <math.h>

  /* The CylSeg class defines oriented segments in the cylindrical plane {z,h=sqrt(x*x+y*y)}.
      {zA,yA} and {zB,yB} are the coordinates of the extreme points.
      detv=zA*yB-zB*yA checks triviality: detv=0 corresponds to an uninteresting segment,
      either trivial or aligned with the interaction point.
      Points {z,y} in the segment satisfy the equation zeqn*z+yeqn*y=1.
      The case zeqn*z+yeqn*y=0 is irrelevant (aligned with IP).
      flysign is the sign determining the orientation of the segment. */

class CylSeg{
    double zA,yA,zB,yB,zeqn,yeqn,detv;
    int flysign;
  public:
    CylSeg(std::array<double,2>,std::array<double,2>,int);
    double DecProb(double,double);
};


#endif
