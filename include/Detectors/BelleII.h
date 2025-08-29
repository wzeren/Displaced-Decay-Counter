#ifndef _DBelleII_
#define _DBelleII_

#include "include/CDetector.h"
#include "HepMC/IO_GenEvent.h"

Detector BelleII();

bool BelleIICuts(HepMC::GenEvent* evtInput);

#endif
