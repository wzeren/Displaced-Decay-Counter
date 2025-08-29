#ifndef _DBelleIILGAZELLEB2_
#define _DBelleIILGAZELLEB2_

#include "include/CDetector.h"
#include "HepMC/IO_GenEvent.h"

Detector BelleIILGAZELLEB2();

bool BelleIILGAZELLEB2Cuts(HepMC::GenEvent* evtInput);

#endif
