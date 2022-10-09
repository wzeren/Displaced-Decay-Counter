/*! \file
 * Definition of the analysis class, giving access to the test functions for the analysis of events.
*/
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
#include "include/CInputInterface.h"

//#include "Pythia.h"
#include "Pythia8/Pythia.h"

//#include "include/decayProbabilitiesHepMC.h"

#include "HepMC/IO_GenEvent.h"
#include "HepMC/GenEvent.h"
//#include "HepMC/ParticleData.h"
#include "HepMC/SimpleVector.h"

/*! \brief
 * The analysis class gives access to the test functions for an event file containing identified LLPs.
*/

class analysis {
public:
    analysis();
    ~analysis() {delete pythia;};    

    void setAllInput(inputInterface input);
    void setINPUTFILEFORMAT(std::string input_file_format_in) {input_file_format = input_file_format_in;}; 
    void setINPUTFILEPATH(std::string input_file_path_in) {input_file_path = input_file_path_in;}; 
    void setSIGMA(double sigma_in) {sigma = sigma_in;};

    void setDETECTORS(std::vector <std::tuple<std::string,double>> myDetectorList_in) {myDetectorList = myDetectorList_in;};
    void setVerbose() {verbose = true;};

    //    void setLLPPID(double LLPPID_in) {LLPPID = LLPPID_in;};
    //    void setMASS(double mass_in) {mass = mass_in;};
    //    void setCTAU(double ctau_in) {ctau = ctau_in;};
    //    void setVISIBLEBR(double visibleBR_in) {visibleBR = visibleBR_in;};
    bool doCalculations(); //< evaluates widths 
    bool initPythia(); //< Initialises Pythia, if needed. 
    bool runPythia(const int nEventsMC, const std::string pathToResultFile);

/*! \brief
 * The CLLP class stores the characteristics of a given LLP.
*/

  class CLLP{
  public:
    int LLPPID{};
    double mass{};
    double ctau{}; 
    double visibleBR {1.};      
  };

  std::vector <CLLP> LLPdata;

    
private: 

    Pythia8::Pythia* pythia; 
    bool verbose; //< declares amount of Information Pythia writes
    std::string input_file_format;
    std::string input_file_path; 

    int nMC;
    double sigma;
    
    //    int LLPPID; 
    //    double mass;
    //    double ctau;
    //double visibleBR;

    std::vector <std::tuple<std::string,double>> myDetectorList;
    double ProducedLLP;

    bool isLast_hepmc(HepMC::GenEvent::particle_const_iterator p , int PID);//input: HepMC particle and PID, output: check if the input is the last particle that "decays to itself"

};
#endif
