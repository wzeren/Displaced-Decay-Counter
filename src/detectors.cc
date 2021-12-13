#include "include/detectors.h"


//**************	START: DETECTOR IMPLEMENTATION	***************

//			PRELIMINARY: define objects

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

//			DETECTOR IMPLEMENTATION

//	A: MATHUSLA

    // Uncle Simon's MATHUSLA
    
Detector MATHUSLA0() {
 std::array<double,2> AA={68.,60.},BB={168.,60.},CC={168.,85.},DD={68.,85.};       // Corner points
 std::vector<std::array<double,2>> ptlist={AA,BB,CC,DD};
// CylSeg mathusl1(AA,BB,1),mathusl2(BB,CC,-1),mathusl3(CC,DD,-1),mathusl4(DD,AA,1);
// std::vector<CylSeg> mathuslist={mathusl1,mathusl2,mathusl3,mathusl4};
 double mathusweight=2.*atan(50./60.)/(8.*atan(1.));                               // Angular aperture
//    CylDetLayer mathuslay(mathuslist,mathusweight);g
 CylDetLayer mathuslay(ptlist,mathusweight);
 std::vector<CylDetLayer> MathuLayers={mathuslay};
 std::string Dname="MATHUSLA0";
 Detector myMATHUSLA(Dname,MathuLayers);
 return myMATHUSLA;
}

    // Improved MATHUSLA
    
Detector MATHUSLA1() {
 // First layer
 std::array<double,2> AA={68.,61.4118},BB={168.,61.4118},CC={168.,87.},DD={68.,87.};
 std::vector<std::array<double,2>> ptlist={AA,BB,CC,DD};
 double mathusweight=0.608826/(8.*atan(1.));
 CylDetLayer mathuslay1(ptlist,mathusweight);
 std::vector<CylDetLayer> IMathuLayers={mathuslay1};
 // Second layer
 AA={68.,64.3714},BB={168.,64.3714},CC={168.,91.1928},DD={68.,91.1928};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.245427/(8.*atan(1.));
 CylDetLayer mathuslay2(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay2);
 // Third layer
 AA={68.,67.4736},BB={168.,67.4736},CC={168.,95.5876},DD={68.,95.5876};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.183756/(8.*atan(1.));
 CylDetLayer mathuslay3(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay3);
 // Fourth layer
 AA={68.,69.811},BB={168.,69.811},CC={168.,98.8988},DD={68.,98.8988};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.069278/(8.*atan(1.));
 CylDetLayer mathuslay4(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay4);
 // Fifth layer
 AA={68.,71.7475},BB={168.,71.7475},CC={168.,91.2912},DD={68.,91.2912};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.104765/(8.*atan(1.));
 CylDetLayer mathuslay5(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay5);
 // Sixth layer
 AA={68.,74.5886},BB={168.,74.5886},CC={168.,84.2688},DD={68.,84.2688};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.117983/(8.*atan(1.));
 CylDetLayer mathuslay6(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay6);
 // Seventh layer
 AA={68.,77.1641},BB={168.,77.1641},CC={168.,79.5382},DD={68.,79.5382};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.0594421/(8.*atan(1.));
 CylDetLayer mathuslay7(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay7);
 std::string Dname="MATHUSLA1";
 Detector myMATHUSLA(Dname,IMathuLayers);
 return myMATHUSLA;
}

    // Building MATHUSLA from 3m-high bricks
    
Detector MATHUSLA2() {
 double xmin=-50., xmax=50., ymin=60., ymax=85., dh=3., dphi=0.00785398, zcoord=118., dl=100.;
 std::vector<CylDetLayer> MathuBricks;
 MathuBricks.clear();
//    std::vector<std::array<double,2>> TDcoord;
//    TDcoord.clear(); 
 int hct0=25, phct0=200;
 for(int hct=0; hct<hct0; hct++){
  int count=0;
  double hcoord=40.+hct*dh;
  for(int phct=0; phct<phct0; phct++){
   double phcoord=atan(1.)+phct*dphi;
   double xcoord=-hcoord*cos(phcoord), ycoord=hcoord*sin(phcoord);   
   if(ycoord>=ymin && ycoord<=ymax && xcoord>=xmin && xcoord<=xmax){
    count=count+1;
   }
  }
  if(count!=0){
   std::array<double,2> brkcoord={zcoord,hcoord};
//       TDcoord.push_back(brkcoord);
   CylDetLayer newbrick=CylBrick(brkcoord,dl,dh,count*dphi,1.);
   MathuBricks.push_back(newbrick);
  }
 }
//     myfile << "coord: " << TDcoord.size() << " , bricks: " << MathuBricks.size() << "\n";
//    for(int i=0;i<TDcoord.size();i++){
//     myfile << "z: " << TDcoord[i][0] << " , h: " << TDcoord[i][1] << "\n";
//    }
 std::string Dname="MATHUSLA2";
 Detector myMATHUSLA(Dname,MathuBricks);
 return myMATHUSLA;
}

//	B: FASER

    // FASER
    
Detector FASER1() {
 std::array<double,2> AA={478.5,0.},BB={480.,0.},CC={480.,0.1},DD={478.5,0.1};       // Corner points
 std::vector<std::array<double,2>> ptlist={AA,BB,CC,DD};
 double faserweight=150./3000.;                                                      // relative luminosity
 CylDetLayer faserlay(ptlist,faserweight);
 std::vector<CylDetLayer> faserLayers={faserlay};
 std::string Dname="FASER";
 Detector myFASER(Dname,faserLayers);
 return myFASER;
}

    // FASER2
    
Detector FASER2() {
 std::array<double,2> AA={475.,0.},BB={480.,0.},CC={480.,1.},DD={475.,1.};           // Corner points
 std::vector<std::array<double,2>> ptlist={AA,BB,CC,DD};
 double faserweight=1.;                                                              // relative luminosity
 CylDetLayer faserlay(ptlist,faserweight);
 std::vector<CylDetLayer> faserLayers={faserlay};
 std::string Dname="FASER2";
 Detector myFASER(Dname,faserLayers);
 return myFASER;
}

//	C: ANUBIS

    // Uncle Simon's ANUBIS
    
Detector ANUBIS0() {
 // First layer
 std::array<double,2> AA={5.,24.},BB={23.,24.},CC={23.,42.667},DD={5.,42.667};
 std::vector<std::array<double,2>> ptlist={AA,BB,CC,DD};
 double anubisweight=2.*atan(9./33.333)/(8.*atan(1.));
 CylDetLayer anubislay1(ptlist,anubisweight);
 std::vector<CylDetLayer> anubisLayers={anubislay1};
 // Second layer
 AA={5.,42.667},BB={23.,42.667},CC={23.,61.333},DD={5.,61.333};
 ptlist={AA,BB,CC,DD};
 anubisweight=2.*atan(9./52.)/(8.*atan(1.));
 CylDetLayer anubislay2(ptlist,anubisweight);
 anubisLayers.push_back(anubislay2);
 // Third layer
 AA={5.,61.333},BB={23.,61.333},CC={23.,80.},DD={5.,80.};
 ptlist={AA,BB,CC,DD};
 anubisweight=2.*atan(9./70.667)/(8.*atan(1.));
 CylDetLayer anubislay3(ptlist,anubisweight);
 anubisLayers.push_back(anubislay3);
 std::string Dname="ANUBIS0";
 Detector myANUBIS(Dname,anubisLayers);
 return myANUBIS;
}

    // Building ANUBIS from 1m-high, 1m-deep bricks
    
Detector ANUBIS1() {
 double ymin=24., ymax=80., xcnt=0., zcnt=14., Rad=9., dphi=0.00392699, dl=1., dh=1.5;
 std::vector<CylDetLayer> AnubisBricks;
 AnubisBricks.clear();
 //   std::vector<std::array<double,3>> TDcoord;
 //   TDcoord.clear(); 
 //   std::ofstream myfile;
 //   myfile.open ("testres.txt", std::ios_base::app);
 int hct0=44, zct0=20, phct0=200;
 for(int hct=0; hct<hct0; hct++){
  double hcoord=24.+hct*dh;
  for(int zct=0; zct<zct0; zct++){
   double zcoord=4.+dl*zct;
   int count=0;
   for(int phct=0; phct<phct0; phct++){
    double phcoord=3./2.*atan(1.)+phct*dphi;
    double xcoord=-hcoord*cos(phcoord), ycoord=hcoord*sin(phcoord);
    double distcnt=sqrt((zcoord-zcnt)*(zcoord-zcnt)+(xcoord-xcnt)*(xcoord-xcnt));
    if(ycoord>=ymin && ycoord<=ymax && distcnt<Rad){
     count=count+1;
    }
   }
   if(count!=0){
    std::array<double,2> brkcoord={zcoord,hcoord};
 //     std::array<double,3> testcoord={zcoord,hcoord,count};
 //     TDcoord.push_back(testcoord);
    CylDetLayer newbrick=CylBrick(brkcoord,dl,dh,count*dphi,1.);
    AnubisBricks.push_back(newbrick);
   }
  }
 }
 //   myfile << "coord: " << TDcoord.size() << " , bricks: " << AnubisBricks.size() << "\n";
 //   for(int i=0;i<TDcoord.size();i++){
 //    myfile << "z: " << TDcoord[i][0] << " , h: " << TDcoord[i][1] << " , count: " << TDcoord[i][2] << "\n";
 //   }
 //   myfile.close();
 std::string Dname="ANUBIS1";
 Detector myANUBIS(Dname,AnubisBricks);
 return myANUBIS;
}

//	D: AL3X

    // AL3X
    
Detector AL3X() {
 std::array<double,2> AA={5.25,0.85},BB={17.25,0.85},CC={17.25,5.},DD={5.25,5.};       // Corner points
 std::vector<std::array<double,2>> ptlist={AA,BB,CC,DD};
 double alexweight=250./3000.;                                   // relative luminosity
 CylDetLayer alexlay(ptlist,alexweight);
 std::vector<CylDetLayer> alexLayers={alexlay};
 std::string Dname="AL3X";
 Detector myAL3X(Dname,alexLayers);
 return myAL3X;
}

//	E: CODEXB

    // Uncle Simon's CODEXB
    
Detector CODEXB0() {
 std::array<double,2> AA={5.,26.},BB={15.,26.},CC={15.,36.},DD={5.,36.};       // Corner points
 std::vector<std::array<double,2>> ptlist={AA,BB,CC,DD};
 double codexweight=2.*atan(5./31.)/(8.*atan(1.));            // Angular aperture
 codexweight=codexweight*300./3000.;                          // relative luminosity
 CylDetLayer codexlay(ptlist,codexweight);
 std::vector<CylDetLayer> codexLayers={codexlay};
 std::string Dname="CODEXB0";
 Detector myCODEXB(Dname,codexLayers);
 return myCODEXB;
}

    // Improved CODEXB
    
Detector CODEXB1() {
 // First layer
 std::array<double,2> AA={5.,26.051358},BB={5.,36.0711111},CC={15.,36.0711111},DD={15.,26.051358};
 std::vector<std::array<double,2>> ptlist={AA,BB,CC,DD};
 double codexweight=0.17766086/(8.*atan(1.));            // Angular aperture
 codexweight=codexweight*300./3000.;                     // relative luminosity
 CylDetLayer codexlay1(ptlist,codexweight);
 std::vector<CylDetLayer> ICodexLayers={codexlay1};
 // Second layer
 AA={5.,26.118785},BB={5.,31.56222},CC={15.,31.56222},DD={15.,26.118785};
 ptlist={AA,BB,CC,DD};
 codexweight=0.0127311073/(8.*atan(1.));                 // Angular aperture
 codexweight=codexweight*300./3000.;                     // relative luminosity
 CylDetLayer codexlay2(ptlist,codexweight);
 ICodexLayers.push_back(codexlay2);
 // Third layer
 AA={5.,26.146760},BB={5.,28.3784804},CC={15.,28.3784804},DD={15.,26.146760};
 ptlist={AA,BB,CC,DD};
 codexweight=0.008702541/(8.*atan(1.));                  // Angular aperture
 codexweight=codexweight*300./3000.;                     // relative luminosity
 CylDetLayer codexlay3(ptlist,codexweight);
 ICodexLayers.push_back(codexlay3);
 // Fourth layer
 AA={5.,26.2479356},BB={5.,36.3432954},CC={15.,36.3432954},DD={15.,26.2479356};
 ptlist={AA,BB,CC,DD};
 codexweight=0.084295998/(8.*atan(1.));                  // Angular aperture
 codexweight=codexweight*300./3000.;                     // relative luminosity
 CylDetLayer codexlay4(ptlist,codexweight);
 ICodexLayers.push_back(codexlay4);
 // Fifth layer
 AA={5.,26.455781},BB={5.,36.6310814},CC={15.,36.6310814},DD={15.,26.455781};
 ptlist={AA,BB,CC,DD};
 codexweight=0.024716089/(8.*atan(1.));                  // Angular aperture
 codexweight=codexweight*300./3000.;                     // relative luminosity
 CylDetLayer codexlay5(ptlist,codexweight);
 ICodexLayers.push_back(codexlay5);
 // Sixth layer
 AA={5.,26.549617},BB={5.,34.596021},CC={15.,34.596021},DD={15.,26.549617};
 ptlist={AA,BB,CC,DD};
 codexweight=0.011806297/(8.*atan(1.));                  // Angular aperture
 codexweight=codexweight*300./3000.;                     // relative luminosity
 CylDetLayer codexlay6(ptlist,codexweight);
 ICodexLayers.push_back(codexlay6);
 // Seventh layer
 AA={5.,26.618576},BB={5.,32.674020},CC={15.,32.674020},DD={15.,26.618576};
 ptlist={AA,BB,CC,DD};
 codexweight=0.012533236/(8.*atan(1.));                  // Angular aperture
 codexweight=codexweight*300./3000.;                     // relative luminosity
 CylDetLayer codexlay7(ptlist,codexweight);
 ICodexLayers.push_back(codexlay7);
 // Eighth layer
 AA={5.,26.696529},BB={5.,30.858797},CC={15.,30.858797},DD={15.,26.696529};
 ptlist={AA,BB,CC,DD};
 codexweight=0.0133093/(8.*atan(1.));                    // Angular aperture
 codexweight=codexweight*300./3000.;                     // relative luminosity
 CylDetLayer codexlay8(ptlist,codexweight);
 ICodexLayers.push_back(codexlay8);
 // Nineth layer
 AA={5.,26.784744},BB={5.,29.144419},CC={15.,29.144419},DD={15.,26.784744};
 ptlist={AA,BB,CC,DD};
 codexweight=0.01413877/(8.*atan(1.));                   // Angular aperture
 codexweight=codexweight*300./3000.;                     // relative luminosity
 CylDetLayer codexlay9(ptlist,codexweight);
 ICodexLayers.push_back(codexlay9);
 // Tenth layer
 AA={5.,26.863778},BB={5.,27.832842},CC={15.,27.832842},DD={15.,26.863778};
 ptlist={AA,BB,CC,DD};
 codexweight=0.00920773/(8.*atan(1.));                   // Angular aperture
 codexweight=codexweight*300./3000.;                     // relative luminosity
 CylDetLayer codexlay10(ptlist,codexweight);
 ICodexLayers.push_back(codexlay10);
 std::string Dname="CODEXB1";
 Detector myCODEXB(Dname,ICodexLayers);
 return myCODEXB;
}

//                   LIST OF DETECTORS

std::vector<Detector> CreateDetectors(std::vector<std::string> nameList) {
 std::vector<Detector> myList,knownDet;
 myList.clear();
 knownDet.clear();
 
//   KNOWN DETECTORS
     // Uncle Simon's MATHUSLA
 Detector MATHUSLAO=MATHUSLA0();
 knownDet.push_back(MATHUSLAO);
    
    // Improved MATHUSLA
 Detector MATHUSLAI=MATHUSLA1();
 knownDet.push_back(MATHUSLAI);
    
    // Building MATHUSLA from 3m-high bricks
 Detector MATHUSLAB=MATHUSLA2();
 knownDet.push_back(MATHUSLAB);
    
    // FASER
 Detector FASERI=FASER1();
 knownDet.push_back(FASERI);
    
    // FASER2
 Detector FASERII=FASER2();
 knownDet.push_back(FASERII);
    
    // Uncle Simon's ANUBIS
 Detector ANUBISO=ANUBIS0();
 knownDet.push_back(ANUBISO);
    
    // Building ANUBIS from 1m-high, 1m-deep bricks
 Detector ANUBISB=ANUBIS1();
 knownDet.push_back(ANUBISB);
    
    // Building AL3X
 Detector AL3X0=AL3X();
 knownDet.push_back(AL3X0);
    
    // Building a simple CODEXB
 Detector CODEXBO=CODEXB0();
 knownDet.push_back(CODEXBO);
    
    // Building an improved CODEXB
 Detector CODEXBI=CODEXB1();
 knownDet.push_back(CODEXBI);
 
//   BUILDING THE LIST OF STUDIED DETECTORS
 
 for(int i=0;i<nameList.size();i++){
  std::string searchname=nameList[i];
  bool foundDet=false;
  for(int j=0;j<knownDet.size();j++){
   std::string foundname=knownDet[j].readname();
   if(foundname==searchname){
    if(!foundDet)myList.push_back(knownDet[j]);
    if(foundDet)std::cout << "Warning! Detector " << searchname << " seems to be defined twice!" << std::endl;
   }
   if(!foundDet)std::cout << "Warning! Detector " << searchname << " does not seem to be declared!" << std::endl;
  }
 }
 
 return myList;
}

//**************	END: DETECTOR IMPLEMENTATION	***************
