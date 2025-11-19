/*! \file
 * Definition of the analyseEvent class, giving access to the cut functions applying to the detectors.
*/
#ifndef _CANALYSEEVENT_
#define _CANALYSEEVENT_

#include "HepMC/IO_GenEvent.h"
#include "HepMC/GenEvent.h"
//#include "HepMC/ParticleData.h"
#include "HepMC/SimpleVector.h"
#include "Pythia8/Pythia.h"
#include "Pythia8Plugins/HepMC2.h"
#include "include/Detectors/MATHUSLA0.h"
#include "include/Detectors/MATHUSLA1.h"
#include "include/Detectors/MATHUSLA2.h"
#include "include/Detectors/FASER1.h"
#include "include/Detectors/FASER2.h"
#include "include/Detectors/AL3X.h"
#include "include/Detectors/CODEXB0.h"
#include "include/Detectors/CODEXB1.h"
#include "include/Detectors/ANUBIS0.h"
#include "include/Detectors/ANUBIS1.h"
#include "include/Detectors/MAPP1.h"
#include "include/Detectors/MAPP2.h"
#include "include/Detectors/FACET.h"
#include "include/Detectors/MATHUSLA40.h"
#include "include/Detectors/ANUBISceiling.h"
#include "include/Detectors/BelleII.h"
#include "include/Detectors/BelleIIBabyGAZELLE.h"
#include "include/Detectors/BelleIIGODZILLA.h"
#include "include/Detectors/BelleIILGAZELLEB1.h"
#include "include/Detectors/BelleIILGAZELLEB2.h"
#include "include/Detectors/SHiPhsds.h"
#include "include/Detectors/FOREHUNT.h"

// END OF INCLUDE DEFINITIONS

/*! \brief
 * The class analyseEvent allows to apply detector cuts to events.
 
 * It is created from an HepMC event and gives access to the passCuts function, checking the cuts that are relevant for a given detector.
*/

class analyseEvent{

 public:
  analyseEvent(HepMC::GenEvent* evtInput){evt=evtInput;};
  ~analyseEvent(){}
  bool passCuts(std::string dettest);

 private:
  HepMC::GenEvent* evt;
  HepMC::GenEvent::particle_const_iterator p;
};

#endif
