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

bool analysis::runPythia(int nEventsMC) {

    if (input_file_format != "LHE" &&  input_file_format != "CMND"){
      return false;
    }
  
    //number of LLPs to be analyzed
    ProducedLLP = 0;
    
    // Creating detector list
    int detTot=myDetectorList.size();
    std::vector<std::string> studiedDet;
    studiedDet.clear();
    std::vector<double> employedLumis;
    employedLumis.clear();
    std::vector<double> observedLLPevents;
    observedLLPevents.clear();
    
    for(int detInd=0;detInd<detTot;detInd++){
     studiedDet.push_back(std::get<0>(myDetectorList[detInd]));
     employedLumis.push_back(std::get<1>(myDetectorList[detInd]));
    }
    std::vector<Detector> DetList=CreateDetectors(studiedDet);

    double foundLumi=0.;
    for(int detInd=0; detInd<detTot; detInd++){
     observedLLPevents.push_back(0.);
     if(employedLumis[detInd]<=0){
      foundLumi=DetList[detInd].readLumi();
      employedLumis[detInd]=foundLumi;
     }
    }
    
    // Loop over Pythia events
    try{
     for (int iEvent = 0; iEvent < nEventsMC; ++iEvent) {
      if (!pythia->next()) continue;
      // Check the list of final state particles
      for (int i = 0; i < pythia->event.size(); ++i) {
       if (abs(pythia->event[i].id()) == LLPPID &&  abs(pythia->event[pythia->event[i].daughter1()].id())!=LLPPID) {
       //search LLPs not decaying into themselves
       //mass = pythia->event[i].m0();
       //ctau = pythia->event[i].tau0()/1000.; //conver mm to m
        ProducedLLP += 1; // count produced LLPs
                        
        Pythia8::Particle XXX=pythia->event[i];
        double gamma = XXX.e()/(mass);
//        double beta_z = XXX.pz()/XXX.e();
        double beta = sqrt(1. - pow(mass/XXX.e(), 2));
        double theta = XXX.p().theta();            
//       double phi = XXX.p().phi();           
//       double eta = XXX.p().eta(); 
        for(int detInd=0; detInd<detTot; detInd++){
         observedLLPevents[detInd] += DetList[detInd].DetAcc(theta,beta*gamma*ctau);
        }
       }
      }
     }
     if(verbose) pythia->stat();   
    }  
    catch(std::exception& e) {
     std::cerr << "!!! Error occured while trying to run Pythia: " << e.what() << std::endl;
     return false;
    }
    
/*  std::ofstream myfile;
    myfile.open ("testres.txt"); //, std::ios_base::app);
    for(int detInd=0; detInd<detTot; detInd++){
     myfile << DetList[detInd].readname() << " : " << observedLLPevents[detInd]*employedLumis[detInd]/3000. << "\n";
    }*/
    
    int nEvent = pythia->mode("Main:numberOfEvents");
    //number of events contained in the sample
    if (nEvent < nEventsMC){
     std::cout << "Warning! You have requested the analysis of " << nEventsMC << " events. But the sample contains only " << nEvent << " events. Resetting the number of events to " << nEvent << " events." << '\n';
    }
    
    //uncomment the line below, to use the cross section included in the input file read by Pythia, for the computation, instead of the user-input value
    //sigma = pythia->info.sigmaGen()*1e12; //in fb  
    
  std::ofstream myfile;
  myfile.open ("Logs/results.txt");
  myfile << "***************************************************************" << "\n";
  myfile << "***************** WELCOME TO THE RESULT FILE ******************" << "\n";
  myfile << "***************************************************************" << "\n";
  myfile << "\n";
  myfile << "***************************************************************" << "\n";
  myfile << "	Number of visible LLP events in the detectors." << "\n";
  myfile << "Detector: simulated acceptance, number of events:" << "\n";
  myfile << "***************************************************************" << "\n";
    for(int detInd=0; detInd<detTot; detInd++){
     double acceptance = observedLLPevents[detInd] / std::max(1.,double(ProducedLLP));
     double VisibleLLPs = observedLLPevents[detInd] * employedLumis[detInd] * sigma * visibleBR / std::max(1.,double(std::min(nEvent,nEventsMC)));
  myfile << DetList[detInd].readname() << " :	" << acceptance << " ,	" << VisibleLLPs << "\n";
    }
  myfile << "\n";
  myfile << "***************************************************************" << "\n";
  myfile << "******************** END OF THE RESULT FILE *******************" << "\n";
  myfile << "***************************************************************" << "\n";
  myfile.close();
    
    std::cout << '\n';
    std::cout << "************* Read the results in bin/Logs/results.txt **************" <<'\n';
    std::cout << '\n';

    return true;
}


bool analysis::runHepMC(int nEventsMC) {
  //JJJ: change to HepMC
  if (input_file_format == "HEPMC"){  
    
    //number of LLPs to be analyzed
      ProducedLLP = 0;
      

    // Creating detector list
    int detTot=myDetectorList.size();
    std::vector<std::string> studiedDet;
    studiedDet.clear();
    std::vector<double> employedLumis;
    employedLumis.clear();
    std::vector<double> observedLLPevents;
    observedLLPevents.clear();
    
    for(int detInd=0;detInd<detTot;detInd++){
     studiedDet.push_back(std::get<0>(myDetectorList[detInd]));
     employedLumis.push_back(std::get<1>(myDetectorList[detInd]));
    }
    std::vector<Detector> DetList=CreateDetectors(studiedDet);

    double foundLumi=0.;
    for(int detInd=0; detInd<detTot; detInd++){
     observedLLPevents.push_back(0.);
     if(employedLumis[detInd]<=0){
      foundLumi=DetList[detInd].readLumi();
      employedLumis[detInd]=foundLumi;
     }
    }

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
	    		
	    		
    double gamma = (*p)->momentum().e()/(mass);
//    double beta_z = (*p)->momentum().pz()/(*p)->momentum().e();
    double beta = sqrt(1. - pow(mass/(*p)->momentum().e(), 2));
    double theta = (*p)->momentum().theta();            
//    double phi = (*p)->momentum().phi();           
//    double eta = (*p)->momentum().eta(); 
        for(int detInd=0; detInd<detTot; detInd++){
         observedLLPevents[detInd] += DetList[detInd].DetAcc(theta,beta*gamma*ctau);
        }
        
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

    
    if (iEvent < nEventsMC){
        std::cout << "Warning! You have requested the analysis of " << nEventsMC << " events. But the sample contains only " << iEvent << " events, but the user requires " << nEventsMC << " events. Resetting the number of events to " << iEvent << " events." << '\n';
    }
//    std::cout <<"iEvent: " << iEvent << '\n';
    //uncomment the line below, to use the cross section included in the input file read by Pythia, for the computation, instead of the user-input value
    //sigma = HepMC::getPythiaCrossSection()*1e12; //in fb  //not working at the moment

    
  std::ofstream myfile;
  myfile.open ("Logs/results.txt");
  myfile << "***************************************************************" << "\n";
  myfile << "***************** WELCOME TO THE RESULT FILE ******************" << "\n";
  myfile << "***************************************************************" << "\n";
  myfile << "\n";
  myfile << "***************************************************************" << "\n";
  myfile << "	Number of visible LLP events in the detectors." << "\n";
  myfile << "Detector: simulated acceptance, number of events:" << "\n";
  myfile << "***************************************************************" << "\n";
    for(int detInd=0; detInd<detTot; detInd++){
     double acceptance = observedLLPevents[detInd] / std::max(1.,double(ProducedLLP));
     double VisibleLLPs = observedLLPevents[detInd] * employedLumis[detInd] * sigma * visibleBR / std::max(1.,double(std::min(iEvent,nEventsMC)));
  myfile << DetList[detInd].readname() << " :	" << acceptance << " ,	" << VisibleLLPs << "\n";
    }
  myfile << "\n";
  myfile << "***************************************************************" << "\n";
  myfile << "******************** END OF THE RESULT FILE *******************" << "\n";
  myfile << "***************************************************************" << "\n";
  myfile.close();
    
    std::cout << '\n';
    std::cout << "************* Read the results in bin/Logs/results.txt **************" <<'\n';
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



