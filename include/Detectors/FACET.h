#ifndef _DFACET_
#define _DFACET_

#include "include/CDetector.h"
#include "HepMC/IO_GenEvent.h"

Detector FACET();

bool FACETCuts(HepMC::GenEvent* evtInput);

#endif
