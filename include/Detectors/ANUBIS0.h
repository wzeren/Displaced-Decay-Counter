#ifndef _DANUBIS0_
#define _DANUBIS0_

#include "include/CDetector.h"
#include "HepMC/IO_GenEvent.h"

Detector ANUBIS0();

bool ANUBIS0Cuts(HepMC::GenEvent* evtInput);

#endif
