#include "include/Detectors/MATHUSLA1.h"

    // Improved MATHUSLA
    
Detector MATHUSLA1() {
 // First layer
 std::array<double,2> AA={68.,60.70588},BB={168.,60.70588},CC={168.,86.},DD={68.,86.};
 std::vector<std::array<double,2>> ptlist={AA,BB,CC,DD};
 double mathusweight=0.43217141/(8.*atan(1.));
 CylDetLayer mathuslay1(ptlist,mathusweight);
 std::vector<CylDetLayer> IMathuLayers={mathuslay1};
 // Second layer
 AA={68.,62.15126},BB={168.,62.15126},CC={168.,88.04762},DD={68.,88.04762};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.17661359/(8.*atan(1.));
 CylDetLayer mathuslay2(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay2);
 // Third layer
 AA={68.,63.63105},BB={168.,63.63105},CC={168.,90.14399},DD={68.,90.14399};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.13389205/(8.*atan(1.));
 CylDetLayer mathuslay3(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay3);
 // Fourth layer
 AA={68.,65.14608},BB={168.,65.14608},CC={168.,92.29028},DD={68.,92.29028};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.11151874/(8.*atan(1.));
 CylDetLayer mathuslay4(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay4);
 // Fifth layer
 AA={68.,66.72300},BB={168.,66.72300},CC={168.,94.52425},DD={68.,94.52425};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.05004968/(8.*atan(1.));
 CylDetLayer mathuslay5(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay5);
 // Sixth layer
 AA={68.,67.99659},BB={168.,67.99659},CC={168.,90.16930},DD={68.,90.16930};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.02513532/(8.*atan(1.));
 CylDetLayer mathuslay6(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay6);
 // Seventh layer
 AA={68.,68.97378},BB={168.,68.97378},CC={168.,86.01499},DD={68.,86.01499};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.02672776/(8.*atan(1.));
 CylDetLayer mathuslay7(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay7);
 // Eighth layer
 AA={68.,70.09776},BB={168.,70.09776},CC={168.,82.05208},DD={68.,82.05208};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.02847502/(8.*atan(1.));
 CylDetLayer mathuslay8(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay8);
 // Nineth layer
 AA={68.,71.39878},BB={168.,71.39878},CC={168.,78.27175},DD={68.,78.27175};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.03040402/(8.*atan(1.));
 CylDetLayer mathuslay9(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay9);
 // Tenth layer
 AA={68.,72.67499},BB={168.,72.67499},CC={168.,75.17543},DD={68.,75.17543};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.02311162/(8.*atan(1.));
 CylDetLayer mathuslay10(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay10);
 // Eleventh layer
 AA={68.,66.54206},BB={168.,66.54206},CC={168.,94.26793},DD={68.,94.26793};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.03928893/(8.*atan(1.));
 CylDetLayer mathuslay11(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay11);
 // Twelfth layer
 AA={68.,67.80652},BB={168.,67.80652},CC={168.,96.05924},DD={68.,96.05924};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.03577842/(8.*atan(1.));
 CylDetLayer mathuslay12(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay12);
 // Thirteenth layer
 AA={68.,69.09501},BB={168.,69.09501},CC={168.,97.88459},DD={68.,97.88459};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.03297591/(8.*atan(1.));
 CylDetLayer mathuslay13(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay13);
 // Fourteenth layer
 AA={68.,70.40798},BB={168.,70.40798},CC={168.,99.74463},DD={68.,99.74463};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.03066688/(8.*atan(1.));
 CylDetLayer mathuslay14(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay14);
 // Fifteenth layer
 AA={68.,72.13752},BB={168.,72.13752},CC={168.,102.19481},DD={68.,102.19481};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.04474024/(8.*atan(1.));
 CylDetLayer mathuslay15(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay15);
 // Sixteenth layer
 AA={68.,73.88663},BB={168.,73.88663},CC={168.,98.70634},DD={68.,98.70634};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.02495037/(8.*atan(1.));
 CylDetLayer mathuslay16(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay16);
 // Seventeenth layer
 AA={68.,75.29893},BB={168.,75.29893},CC={168.,95.33695},DD={68.,95.33695};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.02632578/(8.*atan(1.));
 CylDetLayer mathuslay17(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay17);
 // Eightteenth layer
 AA={68.,76.90665},BB={168.,76.90665},CC={168.,92.08257},DD={68.,92.08257};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.02783794/(8.*atan(1.));
 CylDetLayer mathuslay18(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay18);
 // Nineteenth layer
 AA={68.,78.74995},BB={168.,78.74995},CC={168.,88.93929},DD={68.,88.93929};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.02951235/(8.*atan(1.));
 CylDetLayer mathuslay19(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay19);
 // Twentieth layer
 AA={68.,80.88112},BB={168.,80.88112},CC={168.,85.90330},DD={68.,85.90330};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.03138199/(8.*atan(1.));
 CylDetLayer mathuslay20(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay20);
 // Twenty-first layer
 AA={68.,82.58617},BB={168.,82.58617},CC={168.,83.81405},DD={68.,83.81405};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.01395767/(8.*atan(1.));
 CylDetLayer mathuslay21(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay21);
 std::string Dname="MATHUSLA1";
 double DLumi=3000.;
 Detector myMATHUSLA(Dname,DLumi,IMathuLayers);
 return myMATHUSLA;
}

bool MATHUSLA1Cuts(HepMC::GenEvent* evtin){
 bool cuts=true;
 return cuts;
};
    
/*Detector MATHUSLA1() {
 // First layer
 std::array<double,2> AA={68.,61.4118},BB={168.,61.4118},CC={168.,87.},DD={68.,87.};
 std::vector<std::array<double,2>> ptlist={AA,BB,CC,DD};
 double mathusweight=0.608826/(8.*atan(1.));
 CylDetLayer mathuslay1(ptlist,mathusweight);
 std::vector<CylDetLayer> IMathuLayers={mathuslay1};
 // Second layer
 AA={68.,64.3714},BB={168.,64.3714},CC={168.,91.1928},DD={68.,91.1928};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.245427/(8.*atan(1.));
 CylDetLayer mathuslay2(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay2);
 // Third layer
 AA={68.,67.4736},BB={168.,67.4736},CC={168.,95.5876},DD={68.,95.5876};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.183756/(8.*atan(1.));
 CylDetLayer mathuslay3(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay3);
 // Fourth layer
 AA={68.,69.811},BB={168.,69.811},CC={168.,98.8988},DD={68.,98.8988};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.069278/(8.*atan(1.));
 CylDetLayer mathuslay4(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay4);
 // Fifth layer
 AA={68.,71.7475},BB={168.,71.7475},CC={168.,91.2912},DD={68.,91.2912};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.104765/(8.*atan(1.));
 CylDetLayer mathuslay5(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay5);
 // Sixth layer
 AA={68.,74.5886},BB={168.,74.5886},CC={168.,84.2688},DD={68.,84.2688};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.117983/(8.*atan(1.));
 CylDetLayer mathuslay6(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay6);
 // Seventh layer
 AA={68.,77.1641},BB={168.,77.1641},CC={168.,79.5382},DD={68.,79.5382};
 ptlist={AA,BB,CC,DD};
 mathusweight=0.0594421/(8.*atan(1.));
 CylDetLayer mathuslay7(ptlist,mathusweight);
 IMathuLayers.push_back(mathuslay7);
 std::string Dname="MATHUSLA1";
 Detector myMATHUSLA(Dname,IMathuLayers);
 return myMATHUSLA;
}*/
