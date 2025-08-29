#ifndef _DBelleIIBabyGAZELLE_
#define _DBelleIIBabyGAZELLE_

#include "include/CDetector.h"
#include "HepMC/IO_GenEvent.h"

Detector BelleIIBabyGAZELLE();

bool BelleIIBabyGAZELLECuts(HepMC::GenEvent* evtInput);

#endif
