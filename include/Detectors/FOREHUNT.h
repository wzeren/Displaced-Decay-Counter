#ifndef _DFOREHUNT_
#define _DFOREHUNT_

#include "include/CDetector.h"
#include "HepMC/IO_GenEvent.h"

Detector FOREHUNT();

bool FOREHUNTCuts(HepMC::GenEvent* evtInput);

#endif
