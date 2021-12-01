#include "include/analysis.h"
#include "include/functions.h"
#include <iostream>
#include <fstream>


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
  const double Pi=4.*atan(1.);
  double elInt=0.;                       // exponential contribution initialized to 0.
  if(detv!=0 && th>=0 && th<=Pi && leff>0){      // no need to compute further in the trivial / undefined cases
    double thA=acos(zA/sqrt(yA*yA+zA*zA));       // checks whether the emission angle falls between the
    double thB=acos(zB/sqrt(yB*yB+zB*zB));       //   extreme points
    if(std::min(thA,thB)<=th && std::max(thA,thB)>=th){    
                                         // in case there is an intersection between trajectory and segment
      double lInt=1e10;
      if(abs(th-Pi/2.)>Pi/4.){           // identifies the inverse distance of the intersection from the IP
        lInt=(zeqn+yeqn*tan(th))/sqrt(1.+tan(th)*tan(th));
      } else if(abs(th-Pi/2.)<1e-10) {
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
    
    std::ofstream myfile;
    myfile.open ("testres.txt", std::ios_base::app);
    
    std::array<double,2> AA={68.,60.},BB={168.,60.},CC={168.,85.},DD={68.,85.};
 //   std::array<double,2> AA={68.,60.},BB={68.,60.},CC={68.,585.},DD={68.,85.};
    std::vector<std::array<double,2>> ptlist={AA,BB,CC,DD};
//    CylSeg mathusl1(AA,BB,1),mathusl2(BB,CC,-1),mathusl3(CC,DD,-1),mathusl4(DD,AA,1);
//    std::vector<CylSeg> mathuslist={mathusl1,mathusl2,mathusl3,mathusl4};
    double mathusweight=0.221142;
//    CylDetLayer mathuslay(mathuslist,mathusweight);
    CylDetLayer mathuslay(ptlist,mathusweight);
    std::vector<CylDetLayer> MathuLayers={mathuslay};
    Detector MATHUSLA0(MathuLayers);

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
    myfile << "testres0: " << MATHUSLA0.DetAcc(theta,beta*gamma*ctau) << " , vs.: " << decayProbabilityMATHUSLA(pythia->event[i]) << "\n";
                        
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




