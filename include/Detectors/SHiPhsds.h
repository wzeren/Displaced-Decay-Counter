#ifndef _DSHiPhsds_
#define _DSHiPhsds_

#include "include/CDetector.h"
#include "HepMC/IO_GenEvent.h"

Detector SHiPhsds();

bool SHiPhsdsCuts(HepMC::GenEvent* evtInput);

#endif
