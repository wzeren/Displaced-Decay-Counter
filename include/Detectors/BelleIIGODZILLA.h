#ifndef _DBelleIIGODZILLA_
#define _DBelleIIGODZILLA_

#include "include/CDetector.h"
#include "HepMC/IO_GenEvent.h"

Detector BelleIIGODZILLA();

bool BelleIIGODZILLACuts(HepMC::GenEvent* evtInput);

#endif
