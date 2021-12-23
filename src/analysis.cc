#include "include/analysis.h"
#include "include/functions.h"
#include "include/CCylSeg.h"
#include "include/CCylDetLayer.h"
#include "include/CDetector.h"
#include "include/detectors.h"
#include "include/main.h"
#include <iostream>
#include <fstream>


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

  if (input_file_format != "LHE" &&  input_file_format != "CMND"){
    return false;
    }

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
            pythia->readString("Next:numberCount = 10000");
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

    if (input_file_format != "LHE" &&  input_file_format != "CMND"){
      return false;
    }
  
    //number of LLPs to be analyzed
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
    myfile.open ("testres.txt"); //, std::ios_base::app);
    
    // Creating detector list
    
    std::vector<std::string> studiedDet={"MATHUSLA0","MATHUSLA1","MATHUSLA2","FASER","FASER2","ANUBIS0","ANUBIS1","AL3X","CODEXB0","CODEXB1","MAPP1","MAPP2"};
    std::vector<Detector> DetList=CreateDetectors(studiedDet);
    int detTot=DetList.size();
    std::vector<double> defaultLumis;
    std::vector<double> observedLLPevents;
    observedLLPevents.clear();
    defaultLumis.clear();
    double foundLumi=0.;
    for(int detInd=0; detInd<detTot; detInd++){
     observedLLPevents.push_back(0.);
     foundLumi=DetList[detInd].readLumi();
     defaultLumis.push_back(foundLumi);
    }
  
  
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
    for(int detInd=0; detInd<detTot; detInd++){
     observedLLPevents[detInd] += DetList[detInd].DetAcc(theta,beta*gamma*ctau);
    }
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
    
    for(int detInd=0; detInd<detTot; detInd++){
     myfile << DetList[detInd].readname() << " : " << observedLLPevents[detInd]*defaultLumis[detInd]/3000. << "\n";
    }
    
    myfile << "USMATHUSLA: " << observedLLPinMATHUSLA << "\n";
    myfile << "USFASER: " << observedLLPinFASER1 << " , USFASER2: " << observedLLPinFASER2 << "\n";
    myfile << "USANUBIS: " << observedLLPinANUBIS << "\n";
    myfile << "USAL3X: " << observedLLPinAL3X << "\n";
    myfile << "USCODEXB: " << observedLLPinCODEXb << "\n";
    myfile << "USMAPP1: " << observedLLPinMAPP1 << "\n";
    myfile << "USMAPP2: " << observedLLPinMAPP2 << "\n";

 
    



    double baseline_int_lumi{3000};// in fb^{-1}
    
    int nEvent = pythia->mode("Main:numberOfEvents");//number of events contained in the sample
    if (nEvent < nEvent){
        std::cout << "The event sample contains " << nEvent << " events, but the user requires " << nEvent << " events!" << '\n';
    	die("Event sample containts fewer events than given by user!");
    }
    
    //uncomment the line below, to use the cross section included in the input file read by Pythia, for the computation, instead of the user-input value
    //sigma = pythia->info.sigmaGen()*1e12; //in fb  
    
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
    std::cout << "XS [fb]: " << sigma <<'\n';//in fb
    std::cout << "ReallyProducedLLP for 3/ab: " << ReallyProducedLLP  << '\n';
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
    
    
    myfile.close();

    return true;
}


bool analysis::runHepMC(int nEventsMC, CubicDetector MAPP1,CubicDetector MAPP2) {
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
	    
	      
	    		observedLLPinAL3X       += decayProbabilityAL3X_hepmc(p);
	    		observedLLPinANUBIS     += decayProbabilityANUBIS1_hepmc(p)+decayProbabilityANUBIS2_hepmc(p)+decayProbabilityANUBIS3_hepmc(p);
	    		observedLLPinCODEXb     += decayProbabilityCODEXb_hepmc(p);
	    		observedLLPinFASER1     += decayProbabilityFASER1_hepmc(p);
	    		observedLLPinFASER2     += decayProbabilityFASER2_hepmc(p);
	    		observedLLPinMAPP1      += decayProbabilityMAPP1_hepmc(p,MAPP1);
	    		observedLLPinMAPP2      += decayProbabilityMAPP2_hepmc(p,MAPP2);
	    		observedLLPinMATHUSLA   += decayProbabilityMATHUSLA_hepmc(p);
	  } // for
}
	  ascii_in >> evt;
	  if  (iEvent >= nEventsMC) break;
	} //while loop
	  
      }
      
      catch(std::exception& e) {
	std::cerr << "!!! Error occured while trying to run Pythia: " << e.what() << std::endl;
	return false;
      }



    double baseline_int_lumi{3000};// in fb^{-1}
    //    double ReallyProducedLLP = nLLP * baseline_int_lumi * sigma * k_factor;
    
    if (iEvent < nEventsMC){
        std::cout << "The event sample contains " << iEvent << " events, but the user requires " << nEventsMC << " events!" << '\n';
    	die("Event sample containts fewer events than given by user!");
    }
    std::cout <<"iEvent: " << iEvent << '\n';
    //uncomment the line below, to use the cross section included in the input file read by Pythia, for the computation, instead of the user-input value
    //sigma = HepMC::getPythiaCrossSection()*1e12; //in fb  //not working at the moment
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
    std::cout << "XS [fb]: " << sigma <<'\n';//in fb
    std::cout << "ReallyProducedLLP for 3/ab: " << ReallyProducedLLP  << '\n';
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

    return true;


    }
    
}      


bool analysis::isLast_hepmc(HepMC::GenEvent::particle_const_iterator p, int PID){


    bool daughter_the_same=false;

    
    if( (*p)->end_vertex() ){
     	for ( HepMC::GenVertex::particle_iterator daughters =(*p)->end_vertex()->particles_begin(HepMC::descendants);daughters != (*p)->end_vertex()->particles_end(HepMC::descendants);++daughters ) {
     		if ( (*daughters)->pdg_id() == PID ){
     			daughter_the_same = true; 
     			break;
     		}
     		

     	}
     	
     	
     	
        if (  (*p)->pdg_id() == PID &&   (!daughter_the_same)  ){
        	return true;
        }
        else return false;
    }
     
    else {
    	if (  (*p)->pdg_id()==PID ){
    		return true;
    	}
    	else{
    		return false;
    	}
    }
    


/*
    
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

*/



}



