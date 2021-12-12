#include "include/analysis.h"

//AL3X proposal 1810.03636
double analysis::decayProbabilityAL3X(Pythia8::Particle XXX) {
    const double L_D = 5.25;//horizontal distance from the IP to the detector: 5.25m (4.25+1 = 5.25m)
    const double L_L = 0.85;//vertical distance from the IP to the detector: 0.85m. (the inner radius)
    const double L_d = 12.; // length of the detector is 12m.
    const double L_H = 4.15; //height of the detector is 4.15m  (as 4.15+0.85 = 5m, the outer radius)
    // Identify the kinematic properties of the neutralino
    //Pythia always calculates in GeV
    double gamma = XXX.e()/(mLLP);
    double beta_z = XXX.pz()/XXX.e();
    double beta = sqrt(1. - pow(mLLP/XXX.e(), 2));
    double theta = XXX.p().theta();            
    double phi = XXX.p().phi();           
    double eta = XXX.p().eta(); 

    if (tan(theta) == 0.0) {
        std::cout << "The impossible happened!" << std::endl;
        return 0;
    }   
    double L1 = std::min(std::max(L_D,L_L/tan(theta)), L_D + L_d);
    double L2 = std::min(std::max(L_D,(L_L + L_H)/tan(theta)), L_D + L_d) - L1;
    if (L_L/tan(theta) >= L_D+L_d) // theta too small, neutralino flying too lowly
        return 0;
    if ((L_L + L_H)/tan(theta) <= L_D) //theta too large, neutralino flying too highly
        return 0;
 
    // The probability that the neutralino would decay in the detector is then as follows (Decay law:            
    double decayprobability{};
	decayprobability=250./3000.*exp(-L1/(beta_z*gamma*ctau)) * (1. - exp(-L2/(beta_z*gamma*ctau))); 

    return  decayprobability;
}   


double analysis::decayProbabilityANUBIS1(Pythia8::Particle XXX) {
    const double dh = 5.0;//horizontal distance from the IP to the detector
    const double dv = 24.0;//vertical distance from the IP to the detector
    const double lh = 18.0; // width/length of the detector
    const double lv = 56.0; //height of the detector
    const double lvseg = lv/3.0;//height of each of the three equal segments of the detector


    // Identify the kinematic properties of the neutralino
    //Pythia always calculates in GeV
    double gamma = XXX.e()/(mLLP);
    double beta_z = fabs(XXX.pz()/XXX.e());
    double beta = sqrt(1. - pow(mLLP/XXX.e(), 2));

    double theta = XXX.p().theta();            
    double phi = XXX.p().phi();     

    if (tan(theta) == 0.0) {
        std::cout << "The impossible happened!" << std::endl;
        return 0;
    }      
    double L1 = std::min(std::max(dh,dv/tan(theta)), dh + lh);
    double L2 = std::min(std::max(dh,(dv + lvseg)/tan(theta)), dh + lh) - L1;
    if (dv/tan(theta) >= dh+lh) // theta too small, neutralino flying too lowly
        return 0;
    if ((dv + lvseg)/tan(theta) <= dh) //theta too large, neutralino flying too highly
        return 0;
    // The probability that the neutralino would decay in the detector is then as follows (Decay law:         
    double decayprobability{};   
	decayprobability=0.083942 * exp(-L1/(beta_z*gamma*ctau)) * (1. - exp(-L2/(beta_z*gamma*ctau))); //2*ArcTan[(lh/2)/(lvseg*1/2 + dv)]/(2 \[Pi]) = 0.083942


    return  decayprobability; 
}


double analysis::decayProbabilityANUBIS2(Pythia8::Particle XXX) {
    const double dh = 5.0;//horizontal distance from the IP to the detector
    const double dv = 24.0;//vertical distance from the IP to the detector
    const double lh = 18.0; // width/length of the detector
    const double lv = 56.0; //height of the detector
    const double lvseg = lv/3.0;//height of each of the three equal segments of the detector


    // Identify the kinematic properties of the neutralino
    //Pythia always calculates in GeV
    double gamma = XXX.e()/(mLLP);
    double beta_z = fabs(XXX.pz()/XXX.e());
    double beta = sqrt(1. - pow(mLLP/XXX.e(), 2));

    double theta = XXX.p().theta();            
    double phi = XXX.p().phi();     

    if (tan(theta) == 0.0) {
        std::cout << "The impossible happened!" << std::endl;
        return 0;
    }      
    double L1 = std::min(std::max(dh,(dv + lvseg)/tan(theta)), dh + lh);
    double L2 = std::min(std::max(dh,(dv + 2.0*lvseg)/tan(theta)), dh + lh) - L1;
    if ((dv + lvseg)/tan(theta) >= dh+lh) // theta too small, neutralino flying too lowly
        return 0;
    if ((dv + 2.0*lvseg)/tan(theta) <= dh) //theta too large, neutralino flying too highly
        return 0;
    // The probability that the neutralino would decay in the detector is then as follows (Decay law:         
    double decayprobability{};
	decayprobability=0.0545517 * exp(-L1/(beta_z*gamma*ctau)) * (1. - exp(-L2/(beta_z*gamma*ctau))); //2*ArcTan[(lh/2)/(lvseg*3/2 + dv)]/(2 \[Pi]) = 0.0545517


    return  decayprobability;          
}

double analysis::decayProbabilityANUBIS3(Pythia8::Particle XXX) {
    const double dh = 5.0;//horizontal distance from the IP to the detector
    const double dv = 24.0;//vertical distance from the IP to the detector
    const double lh = 18.0; // width/length of the detector
    const double lv = 56.0; //height of the detector
    const double lvseg = lv/3.0;//height of each of the three equal segments of the detector


    // Identify the kinematic properties of the neutralino
    //Pythia always calculates in GeV
    double gamma = XXX.e()/(mLLP);
    double beta_z = fabs(XXX.pz()/XXX.e());
    double beta = sqrt(1. - pow(mLLP/XXX.e(), 2));

    double theta = XXX.p().theta();            
    double phi = XXX.p().phi();     

    if (tan(theta) == 0.0) {
        std::cout << "The impossible happened!" << std::endl;
        return 0;
    }      
    double L1 = std::min(std::max(dh,(dv + 2.0*lvseg)/tan(theta)), dh + lh);
    double L2 = std::min(std::max(dh,(dv + 3.0*lvseg)/tan(theta)), dh + lh) - L1;
    if ((dv + 2.0*lvseg)/tan(theta) >= dh+lh) // theta too small, neutralino flying too lowly
        return 0;
    if ((dv + 3.0*lvseg)/tan(theta) <= dh) //theta too large, neutralino flying too highly
        return 0;
    // The probability that the neutralino would decay in the detector is then as follows (Decay law:        
    double  decayprobability{};
	decayprobability=0.0403224 * exp(-L1/(beta_z*gamma*ctau)) * (1. - exp(-L2/(beta_z*gamma*ctau))); //2*ArcTan[(lh/2)/(lvseg*5/2 + dv)]/(2 \[Pi]) = 0.0403224


    return  decayprobability;           
}


//CODEX-b proposal paper: 1708.09395. Layout illustrated in Fig.1.
double analysis::decayProbabilityCODEXb(Pythia8::Particle XXX) {
    const double L_D = 25.; //distance from the IP to the detector is ~25m
    const double L_d = 10.; //length of the detector is ~10m.  Not very much exact, but it is the way the original paper handles the probability.

    // Identify the kinematic properties of the neutralino
    //Pythia always calculates in GeV
    double gamma = XXX.e()/(mLLP);
    double beta_z = XXX.pz()/XXX.e();
    double beta = sqrt(1. - pow(mLLP/XXX.e(), 2));
    double theta = XXX.p().theta();            
    double phi = XXX.p().phi();           
    double eta = XXX.p().eta();      

    if (tan(theta) == 0.0) {
        std::cout << "The impossible happened!" << std::endl;
        return 0;
    }   
    if ( (eta < 0.2) || (eta > 0.6) )   //coverage of the pesudo-rapidity of CODEX-b is [0.2,0.6]. 
        return 0;
   
    // The probability that the neutralino would decay in the detector is then as follows (Decay law:        
    double  decayprobability{};
	decayprobability=300./3000.*0.4/3.1415926/2. * exp(-L_D/(beta*gamma*ctau)) * (1. - exp(-L_d/(beta*gamma*ctau))); //  0.4/3.1415926/2 comes from phi coverage: 0.4 / 2pi. Here we use beta instead of beta_z, as we treat the detector as a cone whose tip sits at the IP.//300/3000 for 300/fb / 3000/fb int lumi at LHCb

    return  decayprobability;           
}


//FASER: PhysRevD.99.095011
double analysis::decayProbabilityFASER1(Pythia8::Particle XXX) {
    const double D_TARG_DET = 478.5; // distance from the IP to the near end of FASER2
    const double L_DET = 1.5; //length of detector
    const double Radius = 0.1; //radius
    
    // Identify the kinematic properties of the neutralino
    //Pythia always calculates in GeV
    double gamma = XXX.e()/(mLLP);
    double beta_z = XXX.pz()/XXX.e();
    double beta = sqrt(1. - pow(mLLP/XXX.e(), 2));
    double theta = XXX.p().theta();            
    double phi = XXX.p().phi();           
    double eta = XXX.p().eta();      
    double z_esc_det = Radius/tan(theta);

    // New conservative guess: The LLP must fly right into the detector to be reconstructed
    if(z_esc_det < D_TARG_DET + L_DET)
      return 0;

    // The probability that the LLP would decay in the detector is then as follows (Decay law:            
    double  decayprobability{};
	decayprobability=150./3000.*exp(-D_TARG_DET/(beta_z*gamma*ctau)) * (1. - exp(-L_DET/(beta_z*gamma*ctau))); //150 /fb int lumi

    return  decayprobability;       
}

double analysis::decayProbabilityFASER2(Pythia8::Particle XXX) {
    const double D_TARG_DET = 475.; // distance from the IP to the near end of FASER2
    const double L_DET = 5.; //length of detector
    const double Radius = 1.; //radius: 1m
    
    // Identify the kinematic properties of the neutralino
    //Pythia always calculates in GeV
    double gamma = XXX.e()/(mLLP);
    double beta_z = XXX.pz()/XXX.e();
    double beta = sqrt(1. - pow(mLLP/XXX.e(), 2));
    double theta = XXX.p().theta();            
    double phi = XXX.p().phi();           
    double eta = XXX.p().eta();      
    double z_esc_det = Radius/tan(theta);

    // New conservative guess: The LLP must fly right into the detector to be reconstructed
    if(z_esc_det < D_TARG_DET + L_DET)
      return 0;

    // The probability that the LLP would decay in the detector is then as follows (Decay law:               
    double  decayprobability{};
	decayprobability=exp(-D_TARG_DET/(beta_z*gamma*ctau)) * (1. - exp(-L_DET/(beta_z*gamma*ctau)));

    return  decayprobability;    
}



double analysis::decayProbabilityMAPP1(Pythia8::Particle XXX, CubicDetector detector) {
    /*
    const double L_D = 25.*widthCalculator::me; //distance from the IP to the detector is ~25m
    const double L_d = 10.*widthCalculator::me; //length of the detector is ~10m.  Not very much exact, but it is the way the original paper handles the probability.
    */
    // Identify the kinematic properties of the neutralino
    //Pythia always calculates in GeV
    double gamma = XXX.e()/(mLLP);
    double beta_z = XXX.pz()/XXX.e();
    double beta = sqrt(1. - pow(mLLP/XXX.e(), 2));
    double theta = XXX.p().theta();            
    double phi = XXX.p().phi();           
    double eta = XXX.p().eta(); 

    Vertex interactionPoint(0.0,0.0,0.0, "Cartesian");
    Vertex eventDirection(1.0, theta, phi ,"Spherical");
    Line eventLine(interactionPoint,eventDirection);

    if (tan(theta) == 0.0) {
        std::cout << "The impossible happened!" << std::endl;
        return 0;
    }
    if ( !detector.isDetectorHit(eventLine) )
        return 0;

    // The probability that the neutralino would decay in the detector is then as follows (Decay law:       
    double  decayprobability{};
	decayprobability= 30./3000.*exp(-detector.lengthToDetector(eventLine)/(beta*gamma*ctau)) * (1. - exp(-detector.lengthInDetector(eventLine)/(beta*gamma*ctau)));//30/fb
    
    return  decayprobability;
    
}

double analysis::decayProbabilityMAPP2(Pythia8::Particle XXX, CubicDetector detector) {
    // Identify the kinematic properties of the neutralino
    //Pythia always calculates in GeV
    double gamma = XXX.e()/(mLLP);
    double beta_z = XXX.pz()/XXX.e();
    double beta = sqrt(1. - pow(mLLP/XXX.e(), 2));
    double theta = XXX.p().theta();            
    double phi = XXX.p().phi();           
    double eta = XXX.p().eta();   
    
    Vertex interactionPoint(0.0,0.0,0.0, "Cartesian");
    Vertex eventDirection(1.0, theta, phi ,"Spherical");
    Line eventLine(interactionPoint,eventDirection); 

    if (tan(theta) == 0.0) {
        std::cout << "The impossible happened!" << std::endl;
        return 0;
    }   
    if ( !detector.isDetectorHit(eventLine) )  
        return 0;
   
    // The probability that the neutralino would decay in the detector is then as follows (Decay law:        
    double  decayprobability{};
	decayprobability=300./3000.*exp(-detector.lengthToDetector(eventLine)/(beta*gamma*ctau)) * (1. - exp(-detector.lengthInDetector(eventLine)/(beta*gamma*ctau))); //300/fb

    return  decayprobability;           
}


//MATHUSLA  2009.01693
double analysis::decayProbabilityMATHUSLA(Pythia8::Particle XXX) {
    const double L_D = 68.;//horizontal distance from the IP to the detector
    const double L_L = 60.;//vertical distance from the IP to the detector
    const double L_d = 100.; // length of the detector
    const double L_H = 25.; //height of the detector
    // Identify the kinematic properties of the neutralino
    //Pythia always calculates in GeV
    double gamma = XXX.e()/(mLLP);
    double beta_z = XXX.pz()/XXX.e();
    double beta = sqrt(1. - pow(mLLP/XXX.e(), 2));
    double theta = XXX.p().theta();            
    double phi = XXX.p().phi();           
    double eta = XXX.p().eta(); 
    
    if (tan(theta) == 0.0) {
        std::cout << "The impossible happened!" << std::endl;
        return 0;
    }   
    double L1 = std::min(std::max(L_D,L_L/tan(theta)), L_D + L_d);
    double L2 = std::min(std::max(L_D,(L_L + L_H)/tan(theta)), L_D + L_d) - L1;
    if (L_L/tan(theta) >= L_D+L_d) // theta too small, neutralino flying too lowly
        return 0;
    if ((L_L + L_H)/tan(theta) <= L_D) //theta too large, neutralino flying too highly
        return 0;
    // The probability that the neutralino would decay in the detector is then as follows (Decay law:        
    double  decayprobability{};
	decayprobability=0.221142*exp(-L1/(beta_z*gamma*ctau)) * (1. - exp(-L2/(beta_z*gamma*ctau))); //For MATHUSLA at CMS   the azimuthal coverage is ArcTan[50/60]*2   /(2 \[Pi]) = 0.221142

    return  decayprobability;           
}   

