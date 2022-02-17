/*! \file
 * Definition of the member functions of the CylSeg class.
*/
#include "include/CCylSeg.h"

/*!
 * The CylSeg constructor takes coordinates of the extreme points (2-dim. arrays) as input, as well as a sign (stored as an integer) to define the orientation.
*/

CylSeg::CylSeg (std::array<double,2> Azy,std::array<double,2> Bzy,int fls) {
  zA=Azy[0];                            // reads the coordinates
  yA=abs(Azy[1]);
  zB=Bzy[0];
  yB=abs(Bzy[1]);
  flysign=1;                            // defines the orientation
  if(fls<0){flysign=-1;}
  detv=zA*yB-zB*yA;                     // checks the triviality
  zeqn=0.;                              // defines the equation
  yeqn=0.;
  if(detv!=0.){
   zeqn=(yB-yA)/detv;
   yeqn=(zA-zB)/detv;
  }
}

/*!
 * The class function DecProb computes the signed exponential factor from an input corresponding to the angle of emission of the particle and its effective flying distance.
 */

double CylSeg::DecProb (double th,double leff) {
  double elInt=0.;  // exponential contribution initialized to 0.
  if(detv!=0 && th>=0 && th<=4.*atan(1.) && leff>0){      
                    // no need to compute further in the trivial / undefined cases.
    double thA=acos(zA/sqrt(yA*yA+zA*zA)); 
                    // checks whether the emission angle falls between the extreme points.
    double thB=acos(zB/sqrt(yB*yB+zB*zB));
    if(std::min(thA,thB)<=th && std::max(thA,thB)>=th){    
                    // in case there is an intersection between trajectory and segment,
      double lInt=1e10;
      if(abs(th-2.*atan(1.))>atan(1.)){           
                    // identifies the inverse distance of the intersection from the IP.
        lInt=(zeqn+yeqn*tan(th))/sqrt(1.+tan(th)*tan(th));
      } else if(abs(th-2.*atan(1.))<1e-10) {
        lInt=yeqn;
      } else {
        lInt=(yeqn+zeqn/tan(th))/sqrt(1.+1./(tan(th)*tan(th)));
      } 
      if(lInt!=0 && leff>0){             // if the quantities are not trivial,
        lInt=1./abs(lInt);
        elInt=exp(-lInt/leff)*flysign;   // computes the exponential contribution
        if(th==thA || th==thB){ elInt=elInt/2.; }  // and ponders by 1/2 for endpoints.
      }
    }
  }
  return elInt;
}
