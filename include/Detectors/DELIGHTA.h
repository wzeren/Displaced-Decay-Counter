#ifndef _DDELIGHTA_
#define _DDELIGHTA_

#include "include/CDetector.h"
#include "HepMC/IO_GenEvent.h"

Detector DELIGHTA();

bool DELIGHTACuts(HepMC::GenEvent* evtInput);

#endif
