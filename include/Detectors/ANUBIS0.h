#ifndef _DANUBIS0_
#define _DANUBIS0_

#include "HepMC/IO_GenEvent.h"
#include "include/CDetector.h"

Detector ANUBIS0();

bool ANUBIS0Cuts(HepMC::GenEvent* evtInput);

#endif
