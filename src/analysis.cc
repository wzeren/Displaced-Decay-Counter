#include "include/analysis.h"
#include "include/functions.h"
#include "include/CCylSeg.h"
#include "include/CCylDetLayer.h"
#include "include/CDetector.h"
#include "include/detectors.h"
#include <iostream>
#include <fstream>

//#include "include/main.h"

analysis::analysis() {
    pythia = new Pythia8::Pythia("../xmldoc/", false);
    verbose = false;
    mass = 0;  //LLP mass in GeV 
    ctau = 0;//LLP ctau in m
    LLPPID = 0;
    sigma = 1;
    visibleBR = 1;
    nMC = 0;
    ProducedLLP = 0; //total number of produced LLP's in MC
};    

bool analysis::initPythia() {

  if (input_file_format != "LHE" &&  input_file_format != "CMND")
    return false;

  try {
        if (input_file_format == "LHE"){
        	pythia->readString("Beams:frameType = 4");
        	pythia->readString("Beams:LHEF = "+input_file_path);
        }
	else if (input_file_format == "CMND"){
  
  		pythia->readFile(input_file_path);
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

    if (input_file_format != "LHE" &&  input_file_format != "CMND")
      return false;
  
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
    double observedLLPinAL3X0{};
    double observedLLPinCODEXB0{};
    
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
    
    // Building AL3X
    Detector AL3X0=AL3X();
    
    // Building a simple CODEXB
    Detector CODEXBO=CODEXB0();
  
  
    try{
        for (int iEvent = 0; iEvent < nEventsMC; ++iEvent) {
                if (!pythia->next()) continue;
                // Check the list of final state particles
                for (int i = 0; i < pythia->event.size(); ++i) {
                    if (abs(pythia->event[i].id()) == LLPPID &&  abs(pythia->event[pythia->event[i].daughter1()].id())!=LLPPID) {//count LLP  //requiring the last LLP in pythia event record
			//mass = pythia->event[i].m0();
			//ctau = pythia->event[i].tau0()/1000.; //conver mm to m
			ProducedLLP += 1;
                        
                        observedLLPinAL3X       += decayProbabilityAL3X(pythia->event[i]);
                        observedLLPinANUBIS     += decayProbabilityANUBIS1(pythia->event[i])+decayProbabilityANUBIS2(pythia->event[i])+decayProbabilityANUBIS3(pythia->event[i]);
                        observedLLPinCODEXb     += decayProbabilityCODEXb(pythia->event[i]);
                        observedLLPinFASER1     += decayProbabilityFASER1(pythia->event[i]);
                        observedLLPinFASER2     += decayProbabilityFASER2(pythia->event[i]);
                        observedLLPinMAPP1      += decayProbabilityMAPP1(pythia->event[i],MAPP1);
                        observedLLPinMAPP2      += decayProbabilityMAPP2(pythia->event[i],MAPP2);
                        observedLLPinMATHUSLA   += decayProbabilityMATHUSLA(pythia->event[i]);
                        
    Pythia8::Particle XXX=pythia->event[i];
    double gamma = XXX.e()/(mass);
    double beta_z = XXX.pz()/XXX.e();
    double beta = sqrt(1. - pow(mass/XXX.e(), 2));
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
    observedLLPinAL3X0   += AL3X0.DetAcc(theta,beta*gamma*ctau);
    observedLLPinCODEXB0   += CODEXBO.DetAcc(theta,beta*gamma*ctau);
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
    
    myfile << "MATHUSLA: " << observedLLPinMATHUSLA0 << " , vs.: " << observedLLPinMATHUSLA << " , vs.: " << observedLLPinMATHUSLA1 << " , vs.: " << observedLLPinMATHUSLA2 << "\n";
    myfile << "FASER: " << observedLLPinFASERI << " , vs.: " << observedLLPinFASER1 << " , FASER2: " << observedLLPinFASERII << " , vs.: " << observedLLPinFASER2 << "\n";
    myfile << "ANUBIS: " << observedLLPinANUBIS0 << " , vs.: " << observedLLPinANUBIS << " , vs: " << observedLLPinANUBIS1 << "\n";
    myfile << "AL3X: " << observedLLPinAL3X0 << " , vs.: " << observedLLPinAL3X << " , CODEXB: " << observedLLPinCODEXB0 << " , vs.: " << observedLLPinCODEXb << "\n";

 
    

    //double sigma = pythia->info.sigmaGen()*1e12; //in fb  

    double baseline_int_lumi{3000};// in fb^{-1}
    
    int nEvent = pythia->mode("Main:numberOfEvents");//number of events contained in the sample
    if (nEvent < nEventsMC){
    	//die("Event sample containts fewer events than given by user!");
    }
    double ReallyProducedLLP =  baseline_int_lumi * sigma * ProducedLLP/double(std::min(nEvent,nEventsMC));


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
    std::cout << "mass [GeV]: " << mass << '\n';
    std::cout << "ctau [m]: " << ctau << '\n';
    std::cout << "produced LLP: " << ProducedLLP << '\n';  
    std::cout << "produced LLP/NMC: " << ProducedLLP/double(std::min(nEvent,nEventsMC)) << '\n';
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
    if (input_file_format == "LHE"){
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


bool analysis::runHepMC(int nEventsMC, double ctau, CubicDetector MAPP1,CubicDetector MAPP2) {
  //JJJ: change to HepMC
  if (input_file_format == "HEPMC"){  
    
      ProducedLLP = 0;
    
      double observedLLPinAL3X{};
      double observedLLPinANUBIS{};
      double observedLLPinCODEXb{};
      double observedLLPinFASER1{};
      double observedLLPinFASER2{};
      double observedLLPinMAPP1{};
      double observedLLPinMAPP2{};
      double observedLLPinMATHUSLA{};
      

      int iEvent = 0;//count number of events in the sample
      try{
	
	//read the input HepMC file
	//	input_path = "../example_input/mg5/pp2W2eN_5GeV_VvSq1em7/test.hepmc";
	//	HepMC::IO_GenEvent ascii_in(input_path,std::ios::in);

	//	HepMC::IO_GenEvent ascii_in("/home/jsk/hepmc-llp/llp_all_detectors/example_input/py8/test.hepmc",std::ios::in);
	HepMC::IO_GenEvent ascii_in(input_file_path,std::ios::in);
	
	//select an event
	HepMC::GenEvent* evt = ascii_in.read_next_event();


	//loop over events of this HepMC file
	while ( evt ){
	iEvent++;
	  //loop over all partcles in the event evt
	  for ( HepMC::GenEvent::particle_const_iterator p  = evt->particles_begin(); p != evt->particles_end(); ++p ){
	    if(isLast_hepmc(p, LLPPID)){
	      //mass = (*p)->momentum().m();
	      ProducedLLP += 1;
	    }
	      
	    observedLLPinAL3X       += decayProbabilityAL3X_hepmc(p);
	    observedLLPinANUBIS     += decayProbabilityANUBIS1_hepmc(p)+decayProbabilityANUBIS2_hepmc(p)+decayProbabilityANUBIS3_hepmc(p);
	    observedLLPinCODEXb     += decayProbabilityCODEXb_hepmc(p);
	    observedLLPinFASER1     += decayProbabilityFASER1_hepmc(p);
	    observedLLPinFASER2     += decayProbabilityFASER2_hepmc(p);
	    observedLLPinMAPP1      += decayProbabilityMAPP1_hepmc(p,MAPP1);
	    observedLLPinMAPP2      += decayProbabilityMAPP2_hepmc(p,MAPP2);
	    observedLLPinMATHUSLA   += decayProbabilityMATHUSLA_hepmc(p);
	  } // for

	  ascii_in >> evt;
	  if  (iEvent < nEventsMC) break;
	} //while loop
      }
      
      catch(std::exception& e) {
	std::cerr << "!!! Error occured while trying to run Pythia: " << e.what() << std::endl;
	return false;
      }

      //JJJ: where to get the cross section? It must be input for hepmc!
      //double sigma = pythia->info.sigmaGen()*1e12; //in fb  
      
      

    double baseline_int_lumi{3000};// in fb^{-1}
    //    double ReallyProducedLLP = nLLP * baseline_int_lumi * sigma * k_factor;
    
    if (iEvent < nEventsMC){
    	//die("Event sample containts fewer events than given by user!");
    }
    double ReallyProducedLLP = baseline_int_lumi * sigma * ProducedLLP/double(std::min(iEvent,nEventsMC));
    
    

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
    std::cout << "mass [GeV]: " << mass << '\n';
    std::cout << "ctau [m]: " << ctau << '\n';
    std::cout << "produced LLP: " << ProducedLLP << '\n';  
    std::cout << "produced LLP/NMC: " << ProducedLLP/double(std::min(iEvent,nEventsMC)) << '\n';
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

    return true;


    }
    
}      


bool analysis::isLast_hepmc(HepMC::GenEvent::particle_const_iterator p, int PID){
  if( (*p)->end_vertex() ){
    
    //    for ( HepMC::GenVertex::particle_iterator daughters =(*p)->end_vertex()->particles_begin(HepMC::descendants);daughters != (*p)->end_vertex()->particles_end(HepMC::descendants);++daughters ) {

    HepMC::GenVertex::particle_iterator daughters =(*p)->end_vertex()->particles_begin(HepMC::descendants);
    if(daughters != (*p)->end_vertex()->particles_end(HepMC::descendants) )
    {
      if((*daughters)->pdg_id() !=PID){
	return true;
      }
      else
	return false;
    }
    return false;
  }
  return false;
}



