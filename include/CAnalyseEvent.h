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

// END OF INCLUDE DEFINITIONS

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
