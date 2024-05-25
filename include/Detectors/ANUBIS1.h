#ifndef _DANUBIS1_
#define _DANUBIS1_

#include "HepMC/IO_GenEvent.h"
#include "include/CDetector.h"

Detector ANUBIS1();

bool ANUBIS1Cuts(HepMC::GenEvent* evtInput);

#endif
