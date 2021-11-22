#include "include/analysis.h"
#include "include/functions.h"



analysis::analysis() {
    pythia = new Pythia8::Pythia("../xmldoc/", false);
    verbose = false;
    mLLP = 0;  //LLP mass in GeV 
    ctau = 0;//LLP ctau in m
    LLPPID = 0;
    k_factor = 1;
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
                        
                        
                       }
                }
            }
        if(verbose)
            pythia->stat();   
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





    // Results
    std::cout << "nLLP: " << nLLP << '\n';
    std::cout << "mLLP [GeV]: " << mLLP << '\n';
    std::cout << "ctau [m]: " << ctau << '\n';
    std::cout << "XS [fb]: " << sigma <<'\n';//in fb
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
    if (parton_generation == "MG5"){
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
    }
    

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




