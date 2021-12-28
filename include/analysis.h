#ifndef _ANALYSIS
#define _ANALYSIS

#include <exception>
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <math.h>
#include <iterator>
#include <functional> 
#include <algorithm>

//#include "Pythia.h"
#include "Pythia8/Pythia.h"
#include "include/cornerpoint/cubic_detector.h"

//#include "include/decayProbabilitiesHepMC.h"

#include "HepMC/IO_GenEvent.h"
#include "HepMC/GenEvent.h"
//#include "HepMC/ParticleData.h"
#include "HepMC/SimpleVector.h"



 


class analysis {
public:
    analysis();
    ~analysis() {};    

    void setINPUTFILEFORMAT(std::string input_file_format_in) {input_file_format = input_file_format_in;}; 
    void setINPUTFILEPATH(std::string input_file_path_in) {input_file_path = input_file_path_in;}; 
    void setLLPPID(double LLPPID_in) {LLPPID = LLPPID_in;};
    void setMASS(double mass_in) {mass = mass_in;};
    void setCTAU(double ctau_in) {ctau = ctau_in;};
    void setSIGMA(double sigma_in) {sigma = sigma_in;};
    void setVISIBLEBR(double visibleBR_in) {visibleBR = visibleBR_in;};
    void setDETECTORS(std::vector <std::tuple<std::string,double>> myDetectorList_in) {myDetectorList = myDetectorList_in;};
    void setVerbose() {verbose = true;};
    
    bool doCalculations(); //< evaluates widths 
    bool initPythia(); //< Initialises Pythia, if needed. 
    bool runPythia(int nEventsMC, CubicDetector MAPP1,CubicDetector MAPP2);
    bool runHepMC(int nEventsMC, CubicDetector MAPP1,CubicDetector MAPP2);
        
private: 
    

    Pythia8::Pythia* pythia; 
    bool verbose; //< declares amount of Information Pythia writes
    

    
    std::string input_file_format;
    std::string input_file_path; 
    int LLPPID; 
    int nMC;
    double mass;
    double ctau;
    double sigma;
    double visibleBR;
    std::vector <std::tuple<std::string,double>> myDetectorList;
    
    double ProducedLLP;
    

    bool isLast_hepmc(HepMC::GenEvent::particle_const_iterator p , int PID);//input: HepMC particle and PID, output: check if the input is the last particle that "decays to itself"

    
    
    //Decay probabilities for the detectors
    double decayProbabilityAL3X(Pythia8::Particle XXX);//input: pythia particle object, output: decay probability, same below
    double decayProbabilityANUBIS1(Pythia8::Particle XXX);    
    double decayProbabilityANUBIS2(Pythia8::Particle XXX);    
    double decayProbabilityANUBIS3(Pythia8::Particle XXX);  
    double decayProbabilityCODEXb(Pythia8::Particle XXX);
    double decayProbabilityFASER1(Pythia8::Particle XXX);
    double decayProbabilityFASER2(Pythia8::Particle XXX);
    double decayProbabilityMAPP1(Pythia8::Particle XXX, CubicDetector detector);
    double decayProbabilityMAPP2(Pythia8::Particle XXX, CubicDetector detector);
    double decayProbabilityMATHUSLA(Pythia8::Particle XXX);  

    //Decay probabilities for the detectors
    double decayProbabilityAL3X_hepmc(HepMC::GenEvent::particle_const_iterator p);//input: HepMC particle object, output: decay probability, same below
    double decayProbabilityANUBIS1_hepmc(HepMC::GenEvent::particle_const_iterator p);    
    double decayProbabilityANUBIS2_hepmc(HepMC::GenEvent::particle_const_iterator p);    
    double decayProbabilityANUBIS3_hepmc(HepMC::GenEvent::particle_const_iterator p);  
    double decayProbabilityCODEXb_hepmc(HepMC::GenEvent::particle_const_iterator p);
    double decayProbabilityFASER1_hepmc(HepMC::GenEvent::particle_const_iterator p);
    double decayProbabilityFASER2_hepmc(HepMC::GenEvent::particle_const_iterator p);
    double decayProbabilityMAPP1_hepmc(HepMC::GenEvent::particle_const_iterator p, CubicDetector detector);
    double decayProbabilityMAPP2_hepmc(HepMC::GenEvent::particle_const_iterator p, CubicDetector detector);
    double decayProbabilityMATHUSLA_hepmc(HepMC::GenEvent::particle_const_iterator p);  

};
#endif
