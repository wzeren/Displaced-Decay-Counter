#ifndef _DAL3X_
#define _DAL3X_

#include "include/CDetector.h"
#include "HepMC/IO_GenEvent.h"

Detector AL3X();

bool AL3XCuts(HepMC::GenEvent* evtInput);

#endif
