#include "include/analysis.h"
#include "include/functions.h"
#include <iostream>
#include <fstream>


//**************	START: DETECTOR IMPLEMENTATION	***************

//			PRELIMINARY: define objects

      // CylSeg class defining oriented segments in the cylindrical plane {z,y}

class CylSeg{
    double zA,yA,zB,yB,zeqn,yeqn,detv;  // {zA,yA} and {zB,yB} are the coordinates of the extreme points
                                        // detv=zA*yB-zB*yA: detv=0 corresponds to an uninteresting segment,
                                        //   either trivial or aligned with the interaction point
                                        // points {z,y} in the segment satisfy the equation zeqn*z+yeqn*y=1.
                                        //   the case zeqn*z+yeqn*y=0. is irrelevant (aligned with IP)
    int flysign;                        // sign determining the orientation of the segment
  public:
    CylSeg(std::array<double,2>,std::array<double,2>,int);
                                        // constructor from pointers to the coordinates of the extreme
                                        //   points and the orientation
    double DecProb(double,double);      // function computing the exponential factor from the angle of  
                                        //   emission of the particle and effective flying distance
};

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

double CylSeg::DecProb (double th,double leff) {
  double elInt=0.;                       // exponential contribution initialized to 0.
  if(detv!=0 && th>=0 && th<=4.*atan(1.) && leff>0){      // no need to compute further in the trivial / undefined cases
    double thA=acos(zA/sqrt(yA*yA+zA*zA));       // checks whether the emission angle falls between the
    double thB=acos(zB/sqrt(yB*yB+zB*zB));       //   extreme points
    if(std::min(thA,thB)<=th && std::max(thA,thB)>=th){    
                                         // in case there is an intersection between trajectory and segment
      double lInt=1e10;
      if(abs(th-2.*atan(1.))>atan(1.)){           // identifies the inverse distance of the intersection from the IP
        lInt=(zeqn+yeqn*tan(th))/sqrt(1.+tan(th)*tan(th));
      } else if(abs(th-2.*atan(1.))<1e-10) {
        lInt=yeqn;
      } else {
        lInt=(yeqn+zeqn/tan(th))/sqrt(1.+1./(tan(th)*tan(th)));
      } 
      if(lInt!=0 && leff>0){             // if the quantities are not trivial
        lInt=1./abs(lInt);
        elInt=exp(-lInt/leff)*flysign;   // compute the exponential contribution
        if(th==thA || th==thB){ elInt=elInt/2.; }  // ponder by 1/2 for endpoints
      }
    }
  }
  return elInt;
}

      // class CylDetLayer defining a detector layer in cylindrical coordinates

class CylDetLayer{
    std::vector<CylSeg> CylSegList;   // built out of a list of oriented cylindrical segment
    double weight;                    // weighing factor: phi-opening, luminosity, 
                                      //                  efficiency for detection of the LLP decay products
  public:
    CylDetLayer(std::vector<CylSeg>,double);  // straightforward constructor
    CylDetLayer(std::vector<std::array<double,2>>,double); // constructor from a list of coordinates in the {z,y} plane
    double inDetDec(double,double);           // weighed decay probability within the detector layer
};

CylDetLayer::CylDetLayer(std::vector<CylSeg> Seglist,double wgh) {  // direct constructor
  CylSegList=Seglist;
  weight=wgh;
}

CylDetLayer::CylDetLayer(std::vector<std::array<double,2>> ptlist,double wgh) {  // constructor from list of coordinates
 // reset all y-values to positive
    if(ptlist.size()>=1){
     for(int i=0; i<ptlist.size(); i++){
      if(ptlist[i][1]<0)ptlist[i][1]=-ptlist[i][1];
     }
    }
 // check that the list contains three non-aligned points
    bool nonalign=false;
    double detcomp=0.;
    if(ptlist.size()>=3){
     for(int i=0; i<ptlist.size()-2; i++){
      for(int j=i+1; j<ptlist.size()-1; j++){
       if(abs(ptlist[j][0]-ptlist[i][0])>1e-5 || abs(ptlist[j][1]-ptlist[i][1])>1e-5) {
        for(int k=j+1; k<ptlist.size(); k++){
         detcomp=abs((ptlist[j][1]-ptlist[i][1])*(ptlist[k][0]-ptlist[i][0])-(ptlist[j][0]-ptlist[i][0])*(ptlist[k][1]-ptlist[i][1]));
         if(detcomp>1e-10)nonalign=true;
         if(nonalign)break;
        }
       }
       if(nonalign)break;
      }
      if(nonalign)break;
     }
    }
 // find the points with farthest and closest theta aperture
      double thmax=0.,thmin=4.*atan(1.);
      int imax=0,imin=0;
     if(nonalign){
      for(int i=0; i<ptlist.size(); i++){
       if(abs(ptlist[i][0]*ptlist[i][0]+ptlist[i][1]*ptlist[i][1])>1e-10){
        double th=acos(ptlist[i][0]/sqrt(ptlist[i][0]*ptlist[i][0]+ptlist[i][1]*ptlist[i][1]));
        if(th>thmax){
         thmax=th;
         imax=i;
        }
        if(th==thmax && ptlist[i][0]*ptlist[i][0]+ptlist[i][1]*ptlist[i][1]>ptlist[imax][0]*ptlist[imax][0]+ptlist[imax][1]*ptlist[imax][1]){
         thmax=th;
         imax=i;
        }
        if(th<thmin){
         thmin=th;
         imin=i;
        }
        if(th==thmin && ptlist[i][0]*ptlist[i][0]+ptlist[i][1]*ptlist[i][1]<ptlist[imin][0]*ptlist[imin][0]+ptlist[imin][1]*ptlist[imin][1]){
         thmin=th;
         imin=i;
        }
       }
      }
      if(imax==imin){
       nonalign=false;
       std::cout << "Warning! Something is wrong with the CylDetLayer constructor." << std::endl;
      }
     }
 // build the external convex envelope
      std::vector<int> ordlist,ordlist2;
     if(nonalign){
      ordlist.push_back(imax);
      std::vector<int> adjpt;
      int istart=imax;
      int count=0;
      do{
      count=count+1;
      adjpt.clear();
      for(int i=0; i<ptlist.size(); i++){
       bool conv=true;
       if(i==istart || (abs(ptlist[istart][0]-ptlist[i][0])<1e-5 && abs(ptlist[istart][1]-ptlist[i][1])<1e-5))conv=false;
       for(int j=0; j<ptlist.size(); j++){
        if(!conv)break;
        if(j==istart || j==i)continue;
        detcomp=(ptlist[istart][0]-ptlist[i][0])*(ptlist[j][1]-ptlist[i][1])-(ptlist[istart][1]-ptlist[i][1])*(ptlist[j][0]-ptlist[i][0]);
        if(detcomp<-1e-10)conv=false;
        if(abs(detcomp)<1e-10 && (ptlist[istart][0]-ptlist[i][0])*(ptlist[istart][0]-ptlist[i][0])+(ptlist[istart][1]-ptlist[i][1])*(ptlist[istart][1]-ptlist[i][1])<(ptlist[istart][0]-ptlist[j][0])*(ptlist[istart][0]-ptlist[j][0])+(ptlist[istart][1]-ptlist[j][1])*(ptlist[istart][1]-ptlist[j][1]))conv=false; 
       }
       if(conv)adjpt.push_back(i);
      }
      if(adjpt.size()==1){
       ordlist.push_back(adjpt[0]);
       istart=adjpt[0];
      } else {
       std::cout << "Warning! Something is wrong with the CylDetLayer constructor." << std::endl;
      }
      } while(ordlist[ordlist.size()-1]!=imin && count<=ptlist.size());
      
      count=0;
      istart=imin;
      ordlist2.push_back(imin);
      do{
      count=count+1;
      adjpt.clear();
      for(int i=0; i<ptlist.size(); i++){
       bool conv=true;
       if(i==istart || (abs(ptlist[istart][0]-ptlist[i][0])<1e-5 && abs(ptlist[istart][1]-ptlist[i][1])<1e-5))conv=false;
       for(int j=0; j<ptlist.size(); j++){
        if(!conv)break;
        if(j==istart || j==i)continue;
        detcomp=(ptlist[istart][0]-ptlist[i][0])*(ptlist[j][1]-ptlist[i][1])-(ptlist[istart][1]-ptlist[i][1])*(ptlist[j][0]-ptlist[i][0]);
        if(detcomp<-1e-10)conv=false;
        if(abs(detcomp)<1e-10 && (ptlist[istart][0]-ptlist[i][0])*(ptlist[istart][0]-ptlist[i][0])+(ptlist[istart][1]-ptlist[i][1])*(ptlist[istart][1]-ptlist[i][1])<(ptlist[istart][0]-ptlist[j][0])*(ptlist[istart][0]-ptlist[j][0])+(ptlist[istart][1]-ptlist[j][1])*(ptlist[istart][1]-ptlist[j][1]))conv=false; 
       }
       if(conv)adjpt.push_back(i);
      }
      if(adjpt.size()==1){
       ordlist2.push_back(adjpt[0]);
       istart=adjpt[0];
      } else {
       std::cout << "Warning! Something is wrong with the CylDetLayer constructor." << std::endl;
      }
      } while(ordlist2[ordlist2.size()-1]!=imax && count<=ptlist.size());
     }
 // define the segments
  CylSegList.clear();
  if(ordlist.size()>1){
   for(int i=0;i<ordlist.size()-1;i++){
    CylSeg idSeg(ptlist[ordlist[i]],ptlist[ordlist[i+1]],-1);
    CylSegList.push_back(idSeg);
   }
  } else {
    std::cout << "Warning! Something is wrong with the CylDetLayer constructor." << std::endl;
  }
  if(ordlist2.size()>1){
   for(int i=0;i<ordlist2.size()-1;i++){
    CylSeg idSeg(ptlist[ordlist2[i]],ptlist[ordlist2[i+1]],1);
    CylSegList.push_back(idSeg);
   }
  } else {
    std::cout << "Warning! Something is wrong with the CylDetLayer constructor." << std::endl;
  }
  weight=wgh;
}

double CylDetLayer::inDetDec(double th,double leff) {  // sums decay probabilities from the listed cyl. segments
  double Pdec=0.;                                      // and applies an overall weight
  if(CylSegList.size()>0 && th>=0 && th<=4.*atan(1.) && leff>0){
   for(int i=0; i<CylSegList.size(); i++) {
    Pdec=Pdec+CylSegList[i].DecProb(th,leff);
   }
   Pdec=weight*Pdec;
  }
  return Pdec;
}

 /* function CylBrick creating standard bricks in order to fill a volume along the line
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
Detector myDetector(LayerList); */
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

      // class Detector defining a full detector

class Detector{
    std::vector<CylDetLayer> CylLayList;  // built out of a list of detector layers
  public:
    Detector(std::vector<CylDetLayer>);   // straightforward constructor
    double DetAcc(double,double);         // weighed decay probability within the detector layer
};

Detector::Detector(std::vector<CylDetLayer> LayList) {  // direct constructor
  CylLayList=LayList;
}

double Detector::DetAcc(double th,double leff) {  // sums decay probabilities from the listed cyl. layers
  double Pdec=0.;
  if(CylLayList.size()>0 && th>=0 && th<=4.*atan(1.) && leff>0){
   for(int i=0; i<CylLayList.size(); i++) {
    Pdec=Pdec+CylLayList[i].inDetDec(th,leff);
   }
   Pdec=Pdec;
  }
  return Pdec;
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
//    CylDetLayer mathuslay(mathuslist,mathusweight);
 CylDetLayer mathuslay(ptlist,mathusweight);
 std::vector<CylDetLayer> MathuLayers={mathuslay};
 Detector myMATHUSLA(MathuLayers);
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
 Detector myMATHUSLA(IMathuLayers);
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
 Detector myMATHUSLA(MathuBricks);
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
 Detector myFASER(faserLayers);
 return myFASER;
}

    // FASER2
    
Detector FASER2() {
 std::array<double,2> AA={475.,0.},BB={480.,0.},CC={480.,1.},DD={475.,1.};           // Corner points
 std::vector<std::array<double,2>> ptlist={AA,BB,CC,DD};
 double faserweight=1.;                                                              // relative luminosity
 CylDetLayer faserlay(ptlist,faserweight);
 std::vector<CylDetLayer> faserLayers={faserlay};
 Detector myFASER(faserLayers);
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
 Detector myANUBIS(anubisLayers);
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
 Detector myANUBIS(AnubisBricks);
 return myANUBIS;
}

//**************	END: DETECTOR IMPLEMENTATION	***************

analysis::analysis() {
    pythia = new Pythia8::Pythia("../xmldoc/", false);
    verbose = false;
    mLLP = 0;  //LLP mass in GeV 
    ctau = 0;//LLP ctau in m
    LLPPID = 0;
    k_factor = 1;
    visibleBR = 1;
    nLLP = 0;
    ProducedLLP = 0; //total number of produced LLP's in MC
};    





bool analysis::initPythia() {
    try {
    
        if (parton_generation == "MG5"){
        	pythia->readString("Beams:frameType = 4");
        	pythia->readString("Beams:LHEF = "+input_path);
        }
        else if (parton_generation == "PY8"){
  
  		pythia->readFile(input_path);
  	}
  	
  	//allow for very long-lived particles
  	pythia->readString("ResonanceWidths:minWidth = 1.97e-30");
  	
  	
        //set random seed on 
        //pythia->readString("Random:setSeed = on");
        //pythia->readString("Random:seed = 0");//pick new random number seed for each run, based on clock     
        
        //to speed up the simulation. swtich off MPI
        pythia->readString("PartonLevel:MPI = 0");//most of time can be switched off. Speeds up a lot.
        //pythia->readString("PartonLevel:ISR = 0");//Suggest not to turn off ISR
        //pythia->readString("PartonLevel:FSR = 0");//Suggest not to turn off FSR
        //pythia->readString("HadronLevel:Hadronize = 0"); //On or off, depends on your process


        // Some general things regarding output
        if(verbose) {
            pythia->readString("Init:showProcesses = on");
            pythia->readString("Init:showChangedSettings = on");
            //pythia->readString("Main:showChangedSettings = on");
            pythia->readString("Stat:showProcessLevel = on");
            pythia->readString("Stat:showErrors = on");
            pythia->readString("Init:showProcesses = on");
            pythia->readString("Print:quiet = off");
            pythia->readString("Init:showChangedParticleData = on");
            pythia->readString("Next:numberCount = 100000");
        }
        else {
            pythia->readString("Init:showProcesses = off");
            pythia->readString("Init:showChangedSettings = off");
            //pythia->readString("Main:showChangedSettings = off");
            pythia->readString("Stat:showProcessLevel = off");
            pythia->readString("Stat:showErrors = off");
            pythia->readString("Init:showProcesses = off");
            pythia->readString("Print:quiet = on");
            pythia->readString("Init:showChangedSettings = off");
            pythia->readString("Init:showChangedParticleData = off");
        }
        pythia->init(); 
    }
    catch(std::exception& e) {
        std::cerr << "!!! Error occured while trying to initialise Pythia: " << e.what() << std::endl;
        return false;
    }
    return true;
}

bool analysis::runPythia(int nEventsMC, CubicDetector MAPP1,CubicDetector MAPP2) {

    ProducedLLP = 0;
    
    
    double observedLLPinAL3X{};
    double observedLLPinANUBIS{};
    double observedLLPinCODEXb{};
    double observedLLPinFASER1{};
    double observedLLPinFASER2{};
    double observedLLPinMAPP1{};
    double observedLLPinMAPP2{};
    double observedLLPinMATHUSLA{};
    double observedLLPinMATHUSLA0{};
    double observedLLPinMATHUSLA1{};
    double observedLLPinMATHUSLA2{};
    double observedLLPinFASERI{};
    double observedLLPinFASERII{};
    double observedLLPinANUBIS0{};
    double observedLLPinANUBIS1{};
    
    std::ofstream myfile;
    myfile.open ("testres.txt", std::ios_base::app);
    
    // Uncle Simon's MATHUSLA
    Detector MATHUSLAO=MATHUSLA0();
    
    // Improved MATHUSLA
    Detector MATHUSLAI=MATHUSLA1();
    
    // Building MATHUSLA from 3m-high bricks
    Detector MATHUSLAB=MATHUSLA2();
    
    // FASER
    Detector FASERI=FASER1();
    
    // FASER2
    Detector FASERII=FASER2();
    
    // Uncle Simon's ANUBIS
    Detector ANUBISO=ANUBIS0();
    
    // Building ANUBIS from 1m-high, 1m-deep bricks
    Detector ANUBISB=ANUBIS1();

    try{
        for (int iEvent = 0; iEvent < nEventsMC; ++iEvent) {
                if (!pythia->next()) continue;
                // Check the list of final state particles
                for (int i = 0; i < pythia->event.size(); ++i) {
                    if (abs(pythia->event[i].id()) == LLPPID &&  abs(pythia->event[pythia->event[i].daughter1()].id())!=LLPPID) {//count LLP  //requiring the last LLP in pythia event record
			mLLP = pythia->event[i].m0();
			ctau = pythia->event[i].tau0()/1000.; //conver mm to m
                       ProducedLLP += 1;
                        
                        //find the mother of the LLP
                        int mother_index = mother_finder(i, LLPPID);
                        //find nLLP:  number of LLPs in each event
                        nLLP = 0;//initialize to zero at each event i in the loop
                        for (int j = pythia->event[mother_index].daughter1(); j <= pythia->event[mother_index].daughter2(); j++ ){
                        	if (abs(pythia->event[j].id())==LLPPID){
                        		nLLP += 1;
                        	}
                        }
                        
                        
                        observedLLPinAL3X       += decayProbabilityAL3X(pythia->event[i]);
                        observedLLPinANUBIS     += decayProbabilityANUBIS1(pythia->event[i])+decayProbabilityANUBIS2(pythia->event[i])+decayProbabilityANUBIS3(pythia->event[i]);
                        observedLLPinCODEXb     += decayProbabilityCODEXb(pythia->event[i]);
                        observedLLPinFASER1     += decayProbabilityFASER1(pythia->event[i]);
                        observedLLPinFASER2     += decayProbabilityFASER2(pythia->event[i]);
                        observedLLPinMAPP1      += decayProbabilityMAPP1(pythia->event[i],MAPP1);
                        observedLLPinMAPP2      += decayProbabilityMAPP2(pythia->event[i],MAPP2);
                        observedLLPinMATHUSLA   += decayProbabilityMATHUSLA(pythia->event[i]);
                        
    Pythia8::Particle XXX=pythia->event[i];
    double gamma = XXX.e()/(mLLP);
    double beta_z = XXX.pz()/XXX.e();
    double beta = sqrt(1. - pow(mLLP/XXX.e(), 2));
    double theta = XXX.p().theta();            
    double phi = XXX.p().phi();           
    double eta = XXX.p().eta(); 
 //   myfile << "testres0: " << MATHUSLAO.DetAcc(theta,beta*gamma*ctau) << " , vs.: " << decayProbabilityMATHUSLA(pythia->event[i]) << " , vs.: " << MATHUSLAI.DetAcc(theta,beta*gamma*ctau) << "\n";
    observedLLPinMATHUSLA0   += MATHUSLAO.DetAcc(theta,beta*gamma*ctau); 
    observedLLPinMATHUSLA1   += MATHUSLAI.DetAcc(theta,beta*gamma*ctau);
    observedLLPinMATHUSLA2   += MATHUSLAB.DetAcc(theta,beta*gamma*ctau);
    observedLLPinFASERI   += FASERI.DetAcc(theta,beta*gamma*ctau);
    observedLLPinFASERII   += FASERII.DetAcc(theta,beta*gamma*ctau);
    observedLLPinANUBIS0   += ANUBISO.DetAcc(theta,beta*gamma*ctau);
    observedLLPinANUBIS1   += ANUBISB.DetAcc(theta,beta*gamma*ctau);
                       }
                }
            }
 //       if(verbose)
 //           pythia->stat();   
    }  
    catch(std::exception& e) {
        std::cerr << "!!! Error occured while trying to run Pythia: " << e.what() << std::endl;
        return false;
    }
    myfile << "MATHUSLA: " << observedLLPinMATHUSLA0 << " , vs.: " << observedLLPinMATHUSLA << " , vs.: " << observedLLPinMATHUSLA1 << " , vs.: " << observedLLPinMATHUSLA2 << "\n";
    myfile << "FASER: " << observedLLPinFASERI << " , vs.: " << observedLLPinFASER1 << " , FASER2: " << observedLLPinFASERII << " , vs.: " << observedLLPinFASER2 << "\n";
    myfile << "ANUBIS: " << observedLLPinANUBIS0 << " , vs.: " << observedLLPinANUBIS << " , vs: " << observedLLPinANUBIS1 << "\n";

 
    

    double sigma = pythia->info.sigmaGen()*1e12; //in fb  

    double baseline_int_lumi{3000};// in fb^{-1}
    double ReallyProducedLLP = nLLP * baseline_int_lumi * sigma * k_factor;



    double reallyobservedLLPinAL3X		= observedLLPinAL3X  	  / ProducedLLP	* ReallyProducedLLP;
    double reallyobservedLLPinANUBIS		= observedLLPinANUBIS	  / ProducedLLP	* ReallyProducedLLP;
    double reallyobservedLLPinCODEXb		= observedLLPinCODEXb	  / ProducedLLP	* ReallyProducedLLP;
    double reallyobservedLLPinFASER1		= observedLLPinFASER1	  / ProducedLLP	* ReallyProducedLLP;
    double reallyobservedLLPinFASER2		= observedLLPinFASER2    / ProducedLLP	* ReallyProducedLLP;
    double reallyobservedLLPinMAPP1		= observedLLPinMAPP1	  / ProducedLLP	* ReallyProducedLLP;
    double reallyobservedLLPinMAPP2		= observedLLPinMAPP2	  / ProducedLLP	* ReallyProducedLLP;
    double reallyobservedLLPinMATHUSLA	= observedLLPinMATHUSLA  / ProducedLLP	* ReallyProducedLLP;

    double reallyvisibleLLPinAL3X		= observedLLPinAL3X  	  / ProducedLLP	* ReallyProducedLLP * visibleBR;
    double reallyvisibleLLPinANUBIS		= observedLLPinANUBIS	  / ProducedLLP	* ReallyProducedLLP * visibleBR;
    double reallyvisibleLLPinCODEXb		= observedLLPinCODEXb	  / ProducedLLP	* ReallyProducedLLP * visibleBR;
    double reallyvisibleLLPinFASER1		= observedLLPinFASER1	  / ProducedLLP	* ReallyProducedLLP * visibleBR;
    double reallyvisibleLLPinFASER2		= observedLLPinFASER2    / ProducedLLP	* ReallyProducedLLP * visibleBR;
    double reallyvisibleLLPinMAPP1		= observedLLPinMAPP1	  / ProducedLLP	* ReallyProducedLLP * visibleBR;
    double reallyvisibleLLPinMAPP2		= observedLLPinMAPP2	  / ProducedLLP	* ReallyProducedLLP * visibleBR;
    double reallyvisibleLLPinMATHUSLA		= observedLLPinMATHUSLA  / ProducedLLP	* ReallyProducedLLP * visibleBR;





    // Results
    std::cout << "nLLP: " << nLLP << '\n';
    std::cout << "mLLP [GeV]: " << mLLP << '\n';
    std::cout << "ctau [m]: " << ctau << '\n';
    std::cout << "produced LLP: " << ProducedLLP << '\n';  
    std::cout << "produced LLP/NMC: " << ProducedLLP/double(nEventsMC) << '\n';
    std::cout << '\n';
    std::cout << "    observedLLPinAL3X: " << observedLLPinAL3X 	<< '\n';
    std::cout << "  observedLLPinANUBIS: " << observedLLPinANUBIS 	<< '\n';
    std::cout << "  observedLLPinCODEXb: " << observedLLPinCODEXb 	<< '\n';
    std::cout << "  observedLLPinFASER1: " << observedLLPinFASER1 	<< '\n';
    std::cout << "  observedLLPinFASER2: " << observedLLPinFASER2 	<< '\n';
    std::cout << "   observedLLPinMAPP1: " << observedLLPinMAPP1 	<< '\n';
    std::cout << "   observedLLPinMAPP2: " << observedLLPinMAPP2 	<< '\n';
    std::cout << "observedLLPinMATHUSLA: " << observedLLPinMATHUSLA	<< '\n';
    std::cout << '\n';
    std::cout << "    acceptanceAL3X: " << observedLLPinAL3X / ProducedLLP 	<< '\n';
    std::cout << "  acceptanceANUBIS: " << observedLLPinANUBIS / ProducedLLP	<< '\n';
    std::cout << "  acceptanceCODEXb: " << observedLLPinCODEXb / ProducedLLP 	<< '\n';
    std::cout << "  acceptanceFASER1: " << observedLLPinFASER1 / ProducedLLP 	<< '\n';
    std::cout << "  acceptanceFASER2: " << observedLLPinFASER2 / ProducedLLP 	<< '\n';
    std::cout << "   acceptanceMAPP1: " << observedLLPinMAPP1 / ProducedLLP 	<< '\n';
    std::cout << "   acceptanceMAPP2: " << observedLLPinMAPP2 / ProducedLLP 	<< '\n';
    std::cout << "acceptanceMATHUSLA: " << observedLLPinMATHUSLA / ProducedLLP	<< '\n';
    std::cout << '\n';
    if (parton_generation == "MG5"){
    		std::cout << "XS [fb]: " << sigma <<'\n';//in fb
    		std::cout << "visibleBR: " << visibleBR << '\n';
    		std::cout << "ReallyProducedLLP: " << ReallyProducedLLP  << '\n';
        	std::cout << '\n';
		std::cout << "    reallyobservedLLPinAL3X: " << reallyobservedLLPinAL3X 	<< '\n';
    		std::cout << "  reallyobservedLLPinANUBIS: " << reallyobservedLLPinANUBIS 	<< '\n';
    		std::cout << "  reallyobservedLLPinCODEXb: " << reallyobservedLLPinCODEXb 	<< '\n';
    		std::cout << "  reallyobservedLLPinFASER1: " << reallyobservedLLPinFASER1 	<< '\n';
    		std::cout << "  reallyobservedLLPinFASER2: " << reallyobservedLLPinFASER2 	<< '\n';
    		std::cout << "   reallyobservedLLPinMAPP1: " << reallyobservedLLPinMAPP1 	<< '\n';
    		std::cout << "   reallyobservedLLPinMAPP2: " << reallyobservedLLPinMAPP2 	<< '\n';
    		std::cout << "reallyobservedLLPinMATHUSLA: " << reallyobservedLLPinMATHUSLA	<< '\n';
        	std::cout << '\n';
		std::cout << "    reallyvisibleLLPinAL3X: " << reallyvisibleLLPinAL3X 	<< '\n';
    		std::cout << "  reallyvisibleLLPinANUBIS: " << reallyvisibleLLPinANUBIS 	<< '\n';
    		std::cout << "  reallyvisibleLLPinCODEXb: " << reallyvisibleLLPinCODEXb 	<< '\n';
    		std::cout << "  reallyvisibleLLPinFASER1: " << reallyvisibleLLPinFASER1 	<< '\n';
    		std::cout << "  reallyvisibleLLPinFASER2: " << reallyvisibleLLPinFASER2 	<< '\n';
    		std::cout << "   reallyvisibleLLPinMAPP1: " << reallyvisibleLLPinMAPP1 	<< '\n';
    		std::cout << "   reallyvisibleLLPinMAPP2: " << reallyvisibleLLPinMAPP2 	<< '\n';
    		std::cout << "reallyvisibleLLPinMATHUSLA: " << reallyvisibleLLPinMATHUSLA	<< '\n';
    		std::cout << '\n';
    }
    
    myfile.close();

    return true;
}




int analysis::mother_finder(int i, int PID){//i is index of a particle in an event record
		if (abs(pythia->event[pythia->event[i].mother1()].id())==PID){
			return mother_finder(pythia->event[i].mother1(), PID);
		}
		else {return pythia->event[i].mother1();}//return mother particle index
}


//AL3X proposal 1810.03636
double analysis::decayProbabilityAL3X(Pythia8::Particle XXX) {
    const double L_D = 5.25;//horizontal distance from the IP to the detector: 5.25m (4.25+1 = 5.25m)
    const double L_L = 0.85;//vertical distance from the IP to the detector: 0.85m. (the inner radius)
    const double L_d = 12.; // length of the detector is 12m.
    const double L_H = 4.15; //height of the detector is 4.15m  (as 4.15+0.85 = 5m, the outer radius)
    // Identify the kinematic properties of the neutralino
    //Pythia always calculates in GeV
    double gamma = XXX.e()/(mLLP);
    double beta_z = XXX.pz()/XXX.e();
    double beta = sqrt(1. - pow(mLLP/XXX.e(), 2));
    double theta = XXX.p().theta();            
    double phi = XXX.p().phi();           
    double eta = XXX.p().eta(); 

    if (tan(theta) == 0.0) {
        std::cout << "The impossible happened!" << std::endl;
        return 0;
    }   
    double L1 = std::min(std::max(L_D,L_L/tan(theta)), L_D + L_d);
    double L2 = std::min(std::max(L_D,(L_L + L_H)/tan(theta)), L_D + L_d) - L1;
    if (L_L/tan(theta) >= L_D+L_d) // theta too small, neutralino flying too lowly
        return 0;
    if ((L_L + L_H)/tan(theta) <= L_D) //theta too large, neutralino flying too highly
        return 0;
 
    // The probability that the neutralino would decay in the detector is then as follows (Decay law:            
    double decayprobability{};
	decayprobability=250./3000.*exp(-L1/(beta_z*gamma*ctau)) * (1. - exp(-L2/(beta_z*gamma*ctau))); 

    return  decayprobability;
}   


double analysis::decayProbabilityANUBIS1(Pythia8::Particle XXX) {
    const double dh = 5.0;//horizontal distance from the IP to the detector
    const double dv = 24.0;//vertical distance from the IP to the detector
    const double lh = 18.0; // width/length of the detector
    const double lv = 56.0; //height of the detector
    const double lvseg = lv/3.0;//height of each of the three equal segments of the detector


    // Identify the kinematic properties of the neutralino
    //Pythia always calculates in GeV
    double gamma = XXX.e()/(mLLP);
    double beta_z = fabs(XXX.pz()/XXX.e());
    double beta = sqrt(1. - pow(mLLP/XXX.e(), 2));

    double theta = XXX.p().theta();            
    double phi = XXX.p().phi();     

    if (tan(theta) == 0.0) {
        std::cout << "The impossible happened!" << std::endl;
        return 0;
    }      
    double L1 = std::min(std::max(dh,dv/tan(theta)), dh + lh);
    double L2 = std::min(std::max(dh,(dv + lvseg)/tan(theta)), dh + lh) - L1;
    if (dv/tan(theta) >= dh+lh) // theta too small, neutralino flying too lowly
        return 0;
    if ((dv + lvseg)/tan(theta) <= dh) //theta too large, neutralino flying too highly
        return 0;
    // The probability that the neutralino would decay in the detector is then as follows (Decay law:         
    double decayprobability{};   
	decayprobability=0.083942 * exp(-L1/(beta_z*gamma*ctau)) * (1. - exp(-L2/(beta_z*gamma*ctau))); //2*ArcTan[(lh/2)/(lvseg*1/2 + dv)]/(2 \[Pi]) = 0.083942


    return  decayprobability; 
}


double analysis::decayProbabilityANUBIS2(Pythia8::Particle XXX) {
    const double dh = 5.0;//horizontal distance from the IP to the detector
    const double dv = 24.0;//vertical distance from the IP to the detector
    const double lh = 18.0; // width/length of the detector
    const double lv = 56.0; //height of the detector
    const double lvseg = lv/3.0;//height of each of the three equal segments of the detector


    // Identify the kinematic properties of the neutralino
    //Pythia always calculates in GeV
    double gamma = XXX.e()/(mLLP);
    double beta_z = fabs(XXX.pz()/XXX.e());
    double beta = sqrt(1. - pow(mLLP/XXX.e(), 2));

    double theta = XXX.p().theta();            
    double phi = XXX.p().phi();     

    if (tan(theta) == 0.0) {
        std::cout << "The impossible happened!" << std::endl;
        return 0;
    }      
    double L1 = std::min(std::max(dh,(dv + lvseg)/tan(theta)), dh + lh);
    double L2 = std::min(std::max(dh,(dv + 2.0*lvseg)/tan(theta)), dh + lh) - L1;
    if ((dv + lvseg)/tan(theta) >= dh+lh) // theta too small, neutralino flying too lowly
        return 0;
    if ((dv + 2.0*lvseg)/tan(theta) <= dh) //theta too large, neutralino flying too highly
        return 0;
    // The probability that the neutralino would decay in the detector is then as follows (Decay law:         
    double decayprobability{};
	decayprobability=0.0545517 * exp(-L1/(beta_z*gamma*ctau)) * (1. - exp(-L2/(beta_z*gamma*ctau))); //2*ArcTan[(lh/2)/(lvseg*3/2 + dv)]/(2 \[Pi]) = 0.0545517


    return  decayprobability;          
}

double analysis::decayProbabilityANUBIS3(Pythia8::Particle XXX) {
    const double dh = 5.0;//horizontal distance from the IP to the detector
    const double dv = 24.0;//vertical distance from the IP to the detector
    const double lh = 18.0; // width/length of the detector
    const double lv = 56.0; //height of the detector
    const double lvseg = lv/3.0;//height of each of the three equal segments of the detector


    // Identify the kinematic properties of the neutralino
    //Pythia always calculates in GeV
    double gamma = XXX.e()/(mLLP);
    double beta_z = fabs(XXX.pz()/XXX.e());
    double beta = sqrt(1. - pow(mLLP/XXX.e(), 2));

    double theta = XXX.p().theta();            
    double phi = XXX.p().phi();     

    if (tan(theta) == 0.0) {
        std::cout << "The impossible happened!" << std::endl;
        return 0;
    }      
    double L1 = std::min(std::max(dh,(dv + 2.0*lvseg)/tan(theta)), dh + lh);
    double L2 = std::min(std::max(dh,(dv + 3.0*lvseg)/tan(theta)), dh + lh) - L1;
    if ((dv + 2.0*lvseg)/tan(theta) >= dh+lh) // theta too small, neutralino flying too lowly
        return 0;
    if ((dv + 3.0*lvseg)/tan(theta) <= dh) //theta too large, neutralino flying too highly
        return 0;
    // The probability that the neutralino would decay in the detector is then as follows (Decay law:        
    double  decayprobability{};
	decayprobability=0.0403224 * exp(-L1/(beta_z*gamma*ctau)) * (1. - exp(-L2/(beta_z*gamma*ctau))); //2*ArcTan[(lh/2)/(lvseg*5/2 + dv)]/(2 \[Pi]) = 0.0403224


    return  decayprobability;           
}


//CODEX-b proposal paper: 1708.09395. Layout illustrated in Fig.1.
double analysis::decayProbabilityCODEXb(Pythia8::Particle XXX) {
    const double L_D = 25.; //distance from the IP to the detector is ~25m
    const double L_d = 10.; //length of the detector is ~10m.  Not very much exact, but it is the way the original paper handles the probability.

    // Identify the kinematic properties of the neutralino
    //Pythia always calculates in GeV
    double gamma = XXX.e()/(mLLP);
    double beta_z = XXX.pz()/XXX.e();
    double beta = sqrt(1. - pow(mLLP/XXX.e(), 2));
    double theta = XXX.p().theta();            
    double phi = XXX.p().phi();           
    double eta = XXX.p().eta();      

    if (tan(theta) == 0.0) {
        std::cout << "The impossible happened!" << std::endl;
        return 0;
    }   
    if ( (eta < 0.2) || (eta > 0.6) )   //coverage of the pesudo-rapidity of CODEX-b is [0.2,0.6]. 
        return 0;
   
    // The probability that the neutralino would decay in the detector is then as follows (Decay law:        
    double  decayprobability{};
	decayprobability=300./3000.*0.4/3.1415926/2. * exp(-L_D/(beta*gamma*ctau)) * (1. - exp(-L_d/(beta*gamma*ctau))); //  0.4/3.1415926/2 comes from phi coverage: 0.4 / 2pi. Here we use beta instead of beta_z, as we treat the detector as a cone whose tip sits at the IP.//300/3000 for 300/fb / 3000/fb int lumi at LHCb

    return  decayprobability;           
}


//FASER: PhysRevD.99.095011
double analysis::decayProbabilityFASER1(Pythia8::Particle XXX) {
    const double D_TARG_DET = 478.5; // distance from the IP to the near end of FASER2
    const double L_DET = 1.5; //length of detector
    const double Radius = 0.1; //radius
    
    // Identify the kinematic properties of the neutralino
    //Pythia always calculates in GeV
    double gamma = XXX.e()/(mLLP);
    double beta_z = XXX.pz()/XXX.e();
    double beta = sqrt(1. - pow(mLLP/XXX.e(), 2));
    double theta = XXX.p().theta();            
    double phi = XXX.p().phi();           
    double eta = XXX.p().eta();      
    double z_esc_det = Radius/tan(theta);

    // New conservative guess: The LLP must fly right into the detector to be reconstructed
    if(z_esc_det < D_TARG_DET + L_DET)
      return 0;

    // The probability that the LLP would decay in the detector is then as follows (Decay law:            
    double  decayprobability{};
	decayprobability=150./3000.*exp(-D_TARG_DET/(beta_z*gamma*ctau)) * (1. - exp(-L_DET/(beta_z*gamma*ctau))); //150 /fb int lumi

    return  decayprobability;       
}

double analysis::decayProbabilityFASER2(Pythia8::Particle XXX) {
    const double D_TARG_DET = 475.; // distance from the IP to the near end of FASER2
    const double L_DET = 5.; //length of detector
    const double Radius = 1.; //radius: 1m
    
    // Identify the kinematic properties of the neutralino
    //Pythia always calculates in GeV
    double gamma = XXX.e()/(mLLP);
    double beta_z = XXX.pz()/XXX.e();
    double beta = sqrt(1. - pow(mLLP/XXX.e(), 2));
    double theta = XXX.p().theta();            
    double phi = XXX.p().phi();           
    double eta = XXX.p().eta();      
    double z_esc_det = Radius/tan(theta);

    // New conservative guess: The LLP must fly right into the detector to be reconstructed
    if(z_esc_det < D_TARG_DET + L_DET)
      return 0;

    // The probability that the LLP would decay in the detector is then as follows (Decay law:               
    double  decayprobability{};
	decayprobability=exp(-D_TARG_DET/(beta_z*gamma*ctau)) * (1. - exp(-L_DET/(beta_z*gamma*ctau)));

    return  decayprobability;    
}



double analysis::decayProbabilityMAPP1(Pythia8::Particle XXX, CubicDetector detector) {
    /*
    const double L_D = 25.*widthCalculator::me; //distance from the IP to the detector is ~25m
    const double L_d = 10.*widthCalculator::me; //length of the detector is ~10m.  Not very much exact, but it is the way the original paper handles the probability.
    */
    // Identify the kinematic properties of the neutralino
    //Pythia always calculates in GeV
    double gamma = XXX.e()/(mLLP);
    double beta_z = XXX.pz()/XXX.e();
    double beta = sqrt(1. - pow(mLLP/XXX.e(), 2));
    double theta = XXX.p().theta();            
    double phi = XXX.p().phi();           
    double eta = XXX.p().eta(); 

    Vertex interactionPoint(0.0,0.0,0.0, "Cartesian");
    Vertex eventDirection(1.0, theta, phi ,"Spherical");
    Line eventLine(interactionPoint,eventDirection);

    if (tan(theta) == 0.0) {
        std::cout << "The impossible happened!" << std::endl;
        return 0;
    }
    if ( !detector.isDetectorHit(eventLine) )
        return 0;

    // The probability that the neutralino would decay in the detector is then as follows (Decay law:       
    double  decayprobability{};
	decayprobability= 30./3000.*exp(-detector.lengthToDetector(eventLine)/(beta*gamma*ctau)) * (1. - exp(-detector.lengthInDetector(eventLine)/(beta*gamma*ctau)));//30/fb
    
    return  decayprobability;
    
}

double analysis::decayProbabilityMAPP2(Pythia8::Particle XXX, CubicDetector detector) {
    // Identify the kinematic properties of the neutralino
    //Pythia always calculates in GeV
    double gamma = XXX.e()/(mLLP);
    double beta_z = XXX.pz()/XXX.e();
    double beta = sqrt(1. - pow(mLLP/XXX.e(), 2));
    double theta = XXX.p().theta();            
    double phi = XXX.p().phi();           
    double eta = XXX.p().eta();   
    
    Vertex interactionPoint(0.0,0.0,0.0, "Cartesian");
    Vertex eventDirection(1.0, theta, phi ,"Spherical");
    Line eventLine(interactionPoint,eventDirection); 

    if (tan(theta) == 0.0) {
        std::cout << "The impossible happened!" << std::endl;
        return 0;
    }   
    if ( !detector.isDetectorHit(eventLine) )  
        return 0;
   
    // The probability that the neutralino would decay in the detector is then as follows (Decay law:        
    double  decayprobability{};
	decayprobability=300./3000.*exp(-detector.lengthToDetector(eventLine)/(beta*gamma*ctau)) * (1. - exp(-detector.lengthInDetector(eventLine)/(beta*gamma*ctau))); //300/fb

    return  decayprobability;           
}


//MATHUSLA  2009.01693
double analysis::decayProbabilityMATHUSLA(Pythia8::Particle XXX) {
    const double L_D = 68.;//horizontal distance from the IP to the detector
    const double L_L = 60.;//vertical distance from the IP to the detector
    const double L_d = 100.; // length of the detector
    const double L_H = 25.; //height of the detector
    // Identify the kinematic properties of the neutralino
    //Pythia always calculates in GeV
    double gamma = XXX.e()/(mLLP);
    double beta_z = XXX.pz()/XXX.e();
    double beta = sqrt(1. - pow(mLLP/XXX.e(), 2));
    double theta = XXX.p().theta();            
    double phi = XXX.p().phi();           
    double eta = XXX.p().eta(); 
    
    if (tan(theta) == 0.0) {
        std::cout << "The impossible happened!" << std::endl;
        return 0;
    }   
    double L1 = std::min(std::max(L_D,L_L/tan(theta)), L_D + L_d);
    double L2 = std::min(std::max(L_D,(L_L + L_H)/tan(theta)), L_D + L_d) - L1;
    if (L_L/tan(theta) >= L_D+L_d) // theta too small, neutralino flying too lowly
        return 0;
    if ((L_L + L_H)/tan(theta) <= L_D) //theta too large, neutralino flying too highly
        return 0;
    // The probability that the neutralino would decay in the detector is then as follows (Decay law:        
    double  decayprobability{};
	decayprobability=0.221142*exp(-L1/(beta_z*gamma*ctau)) * (1. - exp(-L2/(beta_z*gamma*ctau))); //For MATHUSLA at CMS   the azimuthal coverage is ArcTan[50/60]*2   /(2 \[Pi]) = 0.221142

    return  decayprobability;           
}   




