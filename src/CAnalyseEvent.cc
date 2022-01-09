#include "include/CAnalyseEvent.h"
#include <iostream>

analyseEvent::analyseEvent(Pythia8::Pythia* pythiaInput) : pythia(pythiaInput){
  //  HepMC::GenEvent* hepMCEvent = new HepMC::GenEvent();
  pythiaToHepMC = new HepMC::Pythia8ToHepMC();
  evt = new HepMC::GenEvent();
  pythiaToHepMC->fill_next_event(*pythia, evt);  

}

bool analyseEvent::passCuts(){
  for ( p  = evt->particles_begin(); p != evt->particles_end(); ++p ){

    //apply trivial cut
    if( (*p)->momentum().perp() > -10. ){
      return true;
    }
    
  }
  return true;
}
