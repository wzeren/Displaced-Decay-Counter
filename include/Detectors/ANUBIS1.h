#ifndef _DANUBIS1_
#define _DANUBIS1_

#include "include/CDetector.h"
#include "HepMC/IO_GenEvent.h"

Detector ANUBIS1();

bool ANUBIS1Cuts(HepMC::GenEvent* evtInput);

#endif
