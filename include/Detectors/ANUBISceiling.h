#ifndef _DANUBISceiling_
#define _DANUBISceiling_

#include "include/CDetector.h"
#include "HepMC/IO_GenEvent.h"

Detector ANUBISceiling();

bool ANUBISceilingCuts(HepMC::GenEvent* evtInput);

#endif
