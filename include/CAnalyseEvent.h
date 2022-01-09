#ifndef _CANALYSEEVENT_
#define _CANALYSEEVENT_

#include "HepMC/IO_GenEvent.h"
#include "HepMC/GenEvent.h"
//#include "HepMC/ParticleData.h"
#include "HepMC/SimpleVector.h"
#include "Pythia8/Pythia.h"
#include "Pythia8Plugins/HepMC2.h"

class analyseEvent{

 public:
  analyseEvent(HepMC::GenEvent* evtInput){evt=evtInput;};
  analyseEvent(Pythia8::Pythia* pythiaInput);
  ~analyseEvent(){delete pythiaToHepMC;}
  bool passCuts();

 private:
  HepMC::GenEvent* evt;
  Pythia8::Pythia* pythia; 
  HepMC::GenEvent::particle_const_iterator p;
  HepMC::Pythia8ToHepMC* pythiaToHepMC;
};

#endif
