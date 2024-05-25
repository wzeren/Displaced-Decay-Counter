#ifndef _DFACET_
#define _DFACET_

#include "HepMC/IO_GenEvent.h"
#include "include/CDetector.h"

Detector FACET();

bool FACETCuts(HepMC::GenEvent* evtInput);

#endif
