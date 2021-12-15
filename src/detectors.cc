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
 double DLumi=3000.;
 Detector myMATHUSLA(Dname,DLumi,MathuLayers);
 return myMATHUSLA;
}

    // Improved MATHUSLA
    
Detector MATHUSLA1() {
 // First layer
 std::array<double,2> AA={68.,60.70588},BB={168.,60.70588},CC={168.,86.},DD={68.,86.};
 std::vector<std::array<double,2>> ptlist={AA,BB,CC,DD};
 double mathusweight=0.43217141/(8.*atan(1.));
 CylDetLayer mathuslay1(ptlist,mathusweight);
 std::vector<CylDetLayer> IMathuLayers={mathuslay1};
 // Second layer
 AA={68.,62.15126},BB={168.,62.15126},CC={168.,88.04762},DD={68.,88.04762};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.17661359/(8.*atan(1.));
 CylDetLayer mathuslay2(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay2);
 // Third layer
 AA={68.,63.63105},BB={168.,63.63105},CC={168.,90.14399},DD={68.,90.14399};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.13389205/(8.*atan(1.));
 CylDetLayer mathuslay3(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay3);
 // Fourth layer
 AA={68.,65.14608},BB={168.,65.14608},CC={168.,92.29028},DD={68.,92.29028};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.11151874/(8.*atan(1.));
 CylDetLayer mathuslay4(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay4);
 // Fifth layer
 AA={68.,66.72300},BB={168.,66.72300},CC={168.,94.52425},DD={68.,94.52425};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.05004968/(8.*atan(1.));
 CylDetLayer mathuslay5(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay5);
 // Sixth layer
 AA={68.,67.99659},BB={168.,67.99659},CC={168.,90.16930},DD={68.,90.16930};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.02513532/(8.*atan(1.));
 CylDetLayer mathuslay6(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay6);
 // Seventh layer
 AA={68.,68.97378},BB={168.,68.97378},CC={168.,86.01499},DD={68.,86.01499};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.02672776/(8.*atan(1.));
 CylDetLayer mathuslay7(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay7);
 // Eighth layer
 AA={68.,70.09776},BB={168.,70.09776},CC={168.,82.05208},DD={68.,82.05208};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.02847502/(8.*atan(1.));
 CylDetLayer mathuslay8(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay8);
 // Nineth layer
 AA={68.,71.39878},BB={168.,71.39878},CC={168.,78.27175},DD={68.,78.27175};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.03040402/(8.*atan(1.));
 CylDetLayer mathuslay9(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay9);
 // Tenth layer
 AA={68.,72.67499},BB={168.,72.67499},CC={168.,75.17543},DD={68.,75.17543};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.02311162/(8.*atan(1.));
 CylDetLayer mathuslay10(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay10);
 // Eleventh layer
 AA={68.,66.54206},BB={168.,66.54206},CC={168.,94.26793},DD={68.,94.26793};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.03928893/(8.*atan(1.));
 CylDetLayer mathuslay11(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay11);
 // Twelfth layer
 AA={68.,67.80652},BB={168.,67.80652},CC={168.,96.05924},DD={68.,96.05924};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.03577842/(8.*atan(1.));
 CylDetLayer mathuslay12(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay12);
 // Thirteenth layer
 AA={68.,69.09501},BB={168.,69.09501},CC={168.,97.88459},DD={68.,97.88459};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.03297591/(8.*atan(1.));
 CylDetLayer mathuslay13(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay13);
 // Fourteenth layer
 AA={68.,70.40798},BB={168.,70.40798},CC={168.,99.74463},DD={68.,99.74463};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.03066688/(8.*atan(1.));
 CylDetLayer mathuslay14(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay14);
 // Fifteenth layer
 AA={68.,72.13752},BB={168.,72.13752},CC={168.,102.19481},DD={68.,102.19481};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.04474024/(8.*atan(1.));
 CylDetLayer mathuslay15(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay15);
 // Sixteenth layer
 AA={68.,73.88663},BB={168.,73.88663},CC={168.,98.70634},DD={68.,98.70634};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.02495037/(8.*atan(1.));
 CylDetLayer mathuslay16(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay16);
 // Seventeenth layer
 AA={68.,75.29893},BB={168.,75.29893},CC={168.,95.33695},DD={68.,95.33695};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.02632578/(8.*atan(1.));
 CylDetLayer mathuslay17(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay17);
 // Eightteenth layer
 AA={68.,76.90665},BB={168.,76.90665},CC={168.,92.08257},DD={68.,92.08257};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.02783794/(8.*atan(1.));
 CylDetLayer mathuslay18(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay18);
 // Nineteenth layer
 AA={68.,78.74995},BB={168.,78.74995},CC={168.,88.93929},DD={68.,88.93929};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.02951235/(8.*atan(1.));
 CylDetLayer mathuslay19(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay19);
 // Twentieth layer
 AA={68.,80.88112},BB={168.,80.88112},CC={168.,85.90330},DD={68.,85.90330};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.03138199/(8.*atan(1.));
 CylDetLayer mathuslay20(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay20);
 // Twenty-first layer
 AA={68.,82.58617},BB={168.,82.58617},CC={168.,83.81405},DD={68.,83.81405};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.01395767/(8.*atan(1.));
 CylDetLayer mathuslay21(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay21);
 std::string Dname="MATHUSLA1";
 double DLumi=3000.;
 Detector myMATHUSLA(Dname,DLumi,IMathuLayers);
 return myMATHUSLA;
}
    
/*Detector MATHUSLA1() {
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
}*/

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
 double DLumi=3000.;
 Detector myMATHUSLA(Dname,DLumi,MathuBricks);
 return myMATHUSLA;
}

//	B: FASER

    // FASER
    
Detector FASER1() {
 std::array<double,2> AA={478.5,0.},BB={480.,0.},CC={480.,0.1},DD={478.5,0.1};       // Corner points
 std::vector<std::array<double,2>> ptlist={AA,BB,CC,DD};
 double faserweight=1.;           // angular coverage
 CylDetLayer faserlay(ptlist,faserweight);
 std::vector<CylDetLayer> faserLayers={faserlay};
 std::string Dname="FASER";
 double DLumi=150.;
 Detector myFASER(Dname,DLumi,faserLayers);
 return myFASER;
}

    // FASER2
    
Detector FASER2() {
 std::array<double,2> AA={475.,0.},BB={480.,0.},CC={480.,1.},DD={475.,1.};           // Corner points
 std::vector<std::array<double,2>> ptlist={AA,BB,CC,DD};
 double faserweight=1.;          // angular coverage
 CylDetLayer faserlay(ptlist,faserweight);
 std::vector<CylDetLayer> faserLayers={faserlay};
 std::string Dname="FASER2";
 double DLumi=3000.;
 Detector myFASER(Dname,DLumi,faserLayers);
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
 double DLumi=3000.;
 Detector myANUBIS(Dname,DLumi,anubisLayers);
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
 double DLumi=3000.;
 Detector myANUBIS(Dname,DLumi,AnubisBricks);
 return myANUBIS;
}

//	D: AL3X

    // AL3X
    
Detector AL3X() {
 std::array<double,2> AA={5.25,0.85},BB={17.25,0.85},CC={17.25,5.},DD={5.25,5.};       // Corner points
 std::vector<std::array<double,2>> ptlist={AA,BB,CC,DD};
 double alexweight=1.;                                   // angular coverage
 CylDetLayer alexlay(ptlist,alexweight);
 std::vector<CylDetLayer> alexLayers={alexlay};
 std::string Dname="AL3X";
 double DLumi=250.;
 Detector myAL3X(Dname,DLumi,alexLayers);
 return myAL3X;
}

//	E: CODEXB

    // Uncle Simon's CODEXB
    
Detector CODEXB0() {
 std::array<double,2> AA={5.,26.},BB={15.,26.},CC={15.,36.},DD={5.,36.};       // Corner points
 std::vector<std::array<double,2>> ptlist={AA,BB,CC,DD};
 double codexweight=2.*atan(5./31.)/(8.*atan(1.));            // Angular aperture
 CylDetLayer codexlay(ptlist,codexweight);
 std::vector<CylDetLayer> codexLayers={codexlay};
 std::string Dname="CODEXB0";
 double DLumi=300.;
 Detector myCODEXB(Dname,DLumi,codexLayers);
 return myCODEXB;
}

    // Improved CODEXB
    
Detector CODEXB1() {
 // First layer
 std::array<double,2> AA={5.,26.051358},BB={5.,36.0711111},CC={15.,36.0711111},DD={15.,26.051358};
 std::vector<std::array<double,2>> ptlist={AA,BB,CC,DD};
 double codexweight=0.17766086/(8.*atan(1.));            // Angular aperture
 CylDetLayer codexlay1(ptlist,codexweight);
 std::vector<CylDetLayer> ICodexLayers={codexlay1};
 // Second layer
 AA={5.,26.118785},BB={5.,31.56222},CC={15.,31.56222},DD={15.,26.118785};
 ptlist={AA,BB,CC,DD};
 codexweight=0.0127311073/(8.*atan(1.));                 // Angular aperture
 CylDetLayer codexlay2(ptlist,codexweight);
 ICodexLayers.push_back(codexlay2);
 // Third layer
 AA={5.,26.146760},BB={5.,28.3784804},CC={15.,28.3784804},DD={15.,26.146760};
 ptlist={AA,BB,CC,DD};
 codexweight=0.008702541/(8.*atan(1.));                  // Angular aperture
 CylDetLayer codexlay3(ptlist,codexweight);
 ICodexLayers.push_back(codexlay3);
 // Fourth layer
 AA={5.,26.2479356},BB={5.,36.3432954},CC={15.,36.3432954},DD={15.,26.2479356};
 ptlist={AA,BB,CC,DD};
 codexweight=0.084295998/(8.*atan(1.));                  // Angular aperture
 CylDetLayer codexlay4(ptlist,codexweight);
 ICodexLayers.push_back(codexlay4);
 // Fifth layer
 AA={5.,26.455781},BB={5.,36.6310814},CC={15.,36.6310814},DD={15.,26.455781};
 ptlist={AA,BB,CC,DD};
 codexweight=0.024716089/(8.*atan(1.));                  // Angular aperture
 CylDetLayer codexlay5(ptlist,codexweight);
 ICodexLayers.push_back(codexlay5);
 // Sixth layer
 AA={5.,26.549617},BB={5.,34.596021},CC={15.,34.596021},DD={15.,26.549617};
 ptlist={AA,BB,CC,DD};
 codexweight=0.011806297/(8.*atan(1.));                  // Angular aperture
 CylDetLayer codexlay6(ptlist,codexweight);
 ICodexLayers.push_back(codexlay6);
 // Seventh layer
 AA={5.,26.618576},BB={5.,32.674020},CC={15.,32.674020},DD={15.,26.618576};
 ptlist={AA,BB,CC,DD};
 codexweight=0.012533236/(8.*atan(1.));                  // Angular aperture
 CylDetLayer codexlay7(ptlist,codexweight);
 ICodexLayers.push_back(codexlay7);
 // Eighth layer
 AA={5.,26.696529},BB={5.,30.858797},CC={15.,30.858797},DD={15.,26.696529};
 ptlist={AA,BB,CC,DD};
 codexweight=0.0133093/(8.*atan(1.));                    // Angular aperture
 CylDetLayer codexlay8(ptlist,codexweight);
 ICodexLayers.push_back(codexlay8);
 // Nineth layer
 AA={5.,26.784744},BB={5.,29.144419},CC={15.,29.144419},DD={15.,26.784744};
 ptlist={AA,BB,CC,DD};
 codexweight=0.01413877/(8.*atan(1.));                   // Angular aperture
 CylDetLayer codexlay9(ptlist,codexweight);
 ICodexLayers.push_back(codexlay9);
 // Tenth layer
 AA={5.,26.863778},BB={5.,27.832842},CC={15.,27.832842},DD={15.,26.863778};
 ptlist={AA,BB,CC,DD};
 codexweight=0.00920773/(8.*atan(1.));                   // Angular aperture
 CylDetLayer codexlay10(ptlist,codexweight);
 ICodexLayers.push_back(codexlay10);
 std::string Dname="CODEXB1";
 double DLumi=300.;
 Detector myCODEXB(Dname,DLumi,ICodexLayers);
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
