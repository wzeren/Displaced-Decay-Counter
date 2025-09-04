/*! \file
 * Definition of the member function passCuts of the analyseEvent class, distributing the cut function to each detector.
*/
#include "include/CAnalyseEvent.h"
#include <iostream>

/*! \brief
 * The passCuts function looks for the cuts relevant for each detector and applies it to the tested event.
 
 * The individual cut functions are defined in the files describing the corresponding detectors in src/Detectors/ .
*/

bool analyseEvent::passCuts(std::string dettest){
  bool testres=true;
  
  if(dettest=="MATHUSLA0")testres=MATHUSLA0Cuts(evt);
  
  if(dettest=="MATHUSLA1")testres=MATHUSLA1Cuts(evt);
  
  if(dettest=="MATHUSLA2")testres=MATHUSLA2Cuts(evt);
  
  if(dettest=="FASER1")testres=FASER1Cuts(evt);
  
  if(dettest=="FASER2")testres=FASER2Cuts(evt);
  
  if(dettest=="AL3X")testres=AL3XCuts(evt);
  
  if(dettest=="CODEXB0")testres=CODEXB0Cuts(evt);
  
  if(dettest=="CODEXB1")testres=CODEXB1Cuts(evt);
  
  if(dettest=="ANUBIS0")testres=ANUBIS0Cuts(evt);
  
  if(dettest=="ANUBIS1")testres=ANUBIS1Cuts(evt);
  
  if(dettest=="MAPP1")testres=MAPP1Cuts(evt);
  
  if(dettest=="MAPP2")testres=MAPP2Cuts(evt);
  
  if(dettest=="FACET")testres=FACETCuts(evt);

  if(dettest=="MATHUSLA40")testres=MATHUSLA40Cuts(evt);

  if(dettest=="ANUBISceiling")testres=ANUBISceilingCuts(evt);

  if(dettest=="BelleII")testres=BelleIICuts(evt);

  if(dettest=="BelleIIBabyGAZELLE")testres=BelleIIBabyGAZELLECuts(evt);

  if(dettest=="BelleIIGODZILLA")testres=BelleIIGODZILLACuts(evt);

  if(dettest=="BelleIILGAZELLEB1")testres=BelleIILGAZELLEB1Cuts(evt);

  if(dettest=="BelleIILGAZELLEB2")testres=BelleIILGAZELLEB2Cuts(evt);

  if(dettest=="SHiPhsds")testres=SHiPhsdsCuts(evt);

// APPLYING CUTS TO DETECTORS
  
  return testres;
}
