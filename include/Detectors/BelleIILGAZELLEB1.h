#ifndef _DBelleIILGAZELLEB1_
#define _DBelleIILGAZELLEB1_

#include "include/CDetector.h"
#include "HepMC/IO_GenEvent.h"

Detector BelleIILGAZELLEB1();

bool BelleIILGAZELLEB1Cuts(HepMC::GenEvent* evtInput);

#endif
