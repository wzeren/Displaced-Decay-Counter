/*! \file
 * Definition of the member functions of the Detector class.
*/
#include "include/CDetector.h"

/*! \brief
 * Constructor of the Detector class.

 * The constructor of the Detector class is straightforward. Indeed, depending on the 
    geometry, there is no unique strategy to build the detector out of cylindrical objects.
   The input thus consists of the identifier (string), 
    the default integrated luminosity (double) and a list of cyl. detector layers.
*/

Detector::Detector(std::string myDet,double myLumi,std::vector<CylDetLayer> LayList) {  // direct constructor
  Detname=myDet;
  DetLumi=myLumi;
  CylLayList=LayList;
}

/*!
 * The class function DetAcc computes the detector acceptance from the decy probabilities
    evaluated at the level of the cylindrical detector layers. 
*/

double Detector::DetAcc(double th,double leff) {
  double Pdec=0.;
  if(CylLayList.size()>0 && th>=0 && th<=4.*atan(1.) && leff>0){
   for(int i=0; i<CylLayList.size(); i++) {
    Pdec=Pdec+CylLayList[i].inDetDec(th,leff);
   }
   Pdec=Pdec;
  }
  return Pdec;
}

/*! 
 * The class function readname reads the label of the detector.
*/

std::string Detector::readname() {
 return Detname;
}

/*! 
 * The class function readLumi reads the default integrated luminosity of the detector.
*/

double Detector::readLumi() {
 return DetLumi;
}


/*! 
 * The function CylBrick creates a 'standard brick' as cylindrical detector layer.
   It defines a rectangular shape in the plane {z,h} of given length and height,
     centered on a single coordinate point.
*/
/*  It can be used to fill a volume along the line
      std::vector<CylDetLayer> LayerList;
      LayerList.clear();
      for(int zct=0; zct<zct0; zct++){
       for(int hct=0; hct<hct0; hct++){
        int count=0;
        double zcoord=dl/2.+zct*dl, hcoord=dh/2.+hct*dh;
        for(int phct=0; phct<phct0; phct++){
         double phcoor=phct*dphi;
         double xcoord=hcoord*cos(phcoor), ycoord=hcoord*sin(phcoord);   
         if(zcoord>=zmin && zcoord<=zmax && ycoord>=ymin && ycoord<=ymax && xcoord>=xmin && xcoord<=xmax)count=count+1;
        }
        if(count!=0){
         std::array<double,2> brkcoord={zcoord,hcoord};
         CylDetLayer newbrick=CylBrick(brkcoord,dl,dh,count*dphi,detwgh);
         LayerList.push_back(newbrick);
        }
       }
      }
      Detector myDetector(LayerList);
 */

CylDetLayer CylBrick(std::array<double,2> coord, double length, double height, double apphi, double wgh) { // 'brick' constructor
  std::array<double,2> AA={coord[0]-length/2.,coord[1]-height/2.};
  std::array<double,2> BB={coord[0]+length/2.,coord[1]-height/2.};
  std::array<double,2> CC={coord[0]+length/2.,coord[1]+height/2.};
  std::array<double,2> DD={coord[0]-length/2.,coord[1]+height/2.};
  std::vector<std::array<double,2>> ptlist={AA,BB,CC,DD};
  double wght=wgh*apphi/(8.*atan(1.0));
  CylDetLayer brick(ptlist,wght);
  return brick;
}

