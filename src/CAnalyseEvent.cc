#include "include/CAnalyseEvent.h"
#include <iostream>

bool analyseEvent::passCuts(){
  for ( p  = evt->particles_begin(); p != evt->particles_end(); ++p ){

    /*
    //apply trivial cut
    if( (*p)->momentum().perp() > -10. ){
    std::cout << (*p)->pdg_id() << std::endl;
      return true;
    }

    */

  }
  return true;
}
