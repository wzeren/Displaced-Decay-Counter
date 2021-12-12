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

/* This class serves as a generalised C++ framework to embed specific scenarios of 
 * RPV decay scenarios Meson -> Neutralino + X, Neutralino -> Meson + Y.
 * The user just dictates the scenario to be considered and this class collects the respective
 * masses, decay constants, calculates the respective partial widths and lifetimes and runs Pythia to 
 * numerically find the number of observable neutralino decays.
 */


class analysis {
public:
    analysis();
    ~analysis() {};    

    void setINPUTFILEFORMAT(std::string input_file_format_in) {input_file_format = input_file_format_in;}; 
    void setPARTONGENERATION(std::string parton_generation_in) {parton_generation = parton_generation_in;}; 
    void setINPUTPATH(std::string input_path_in) {input_path = input_path_in;}; 
    void setLLPPID(double LLPPID_in) {LLPPID = LLPPID_in;};
    void setKFACTOR(double k_factor_in) {k_factor = k_factor_in;};
    void setVISIBLEBR(double visibleBR_in) {visibleBR = visibleBR_in;};
    void setVerbose() {verbose = true;};
    
    bool doCalculations(); //< evaluates widths 
    bool initPythia(); //< Initialises Pythia, if needed. 
    bool runPythia(int nEventsMC, CubicDetector MAPP1,CubicDetector MAPP2);
    bool runHepMC(int nEventsMC, double ctau, CubicDetector MAPP1,CubicDetector MAPP2);
        
private: 
    

    Pythia8::Pythia* pythia; 
    bool verbose; //< declares amount of Information Pythia writes
    
    double mLLP; // given in GeV
    double ctau;
    
    std::string parton_generation;
    std::string input_file_format;
    std::string input_path; 
    int LLPPID; 
    double k_factor;
    double visibleBR;
    
    double ProducedLLP;
    
    int mother_finder(int i, int PID);
    bool isLast_hepmc(HepMC::GenEvent::particle_const_iterator p , int PID);
    int nLLP;
    
    
    double decayProbabilityAL3X(Pythia8::Particle XXX);
    double decayProbabilityANUBIS1(Pythia8::Particle XXX);    
    double decayProbabilityANUBIS2(Pythia8::Particle XXX);    
    double decayProbabilityANUBIS3(Pythia8::Particle XXX);  
    double decayProbabilityCODEXb(Pythia8::Particle XXX);
    double decayProbabilityFASER1(Pythia8::Particle XXX);
    double decayProbabilityFASER2(Pythia8::Particle XXX);
    double decayProbabilityMAPP1(Pythia8::Particle XXX, CubicDetector detector);
    double decayProbabilityMAPP2(Pythia8::Particle XXX, CubicDetector detector);
    double decayProbabilityMATHUSLA(Pythia8::Particle XXX);  

    double decayProbabilityAL3X_hepmc(HepMC::GenEvent::particle_const_iterator p);
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
