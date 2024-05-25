#ifndef _DAL3X_
#define _DAL3X_

#include "HepMC/IO_GenEvent.h"
#include "include/CDetector.h"

Detector AL3X();

bool AL3XCuts(HepMC::GenEvent* evtInput);

#endif
