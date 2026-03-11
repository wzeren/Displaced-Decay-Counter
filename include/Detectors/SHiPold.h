#ifndef _DSHiPold_
#define _DSHiPold_

#include "include/CDetector.h"
#include "HepMC/IO_GenEvent.h"

Detector SHiPold();

bool SHiPoldCuts(HepMC::GenEvent* evtInput);

#endif
