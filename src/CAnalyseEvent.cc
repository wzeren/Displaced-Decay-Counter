#include "include/CAnalyseEvent.h"
#include <iostream>

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
  
// APPLYING CUTS TO DETECTORS
  
  return testres;
}
