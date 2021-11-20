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
    
    void setINPUTPATH(std::string input_path_in) {input_path = input_path_in;}; 
    void setLLPPID(double LLPPID_in) {LLPPID = LLPPID_in;};
    void setKFACTOR(double k_factor_in) {k_factor = k_factor_in;};
    void setVerbose() {verbose = true;};
    
    bool doCalculations(); //< evaluates widths 
    bool initPythia(); //< Initialises Pythia, if needed. 
    bool runPythia(int nEventsMC, CubicDetector MAPP1,CubicDetector MAPP2);
    
        
private: 
    

    Pythia8::Pythia* pythia; //< Pythia8 object for simulation
    bool verbose; //< declares amount of Information Pythia writes
    
    double mLLP; // given in GeV
    double ctau;
    

    std::string input_path; 
    int LLPPID; 
    double k_factor{1};
    
    double ProducedLLP;
    
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
   
};
#endif
