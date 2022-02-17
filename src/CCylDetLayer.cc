/*! \file
 * Definition of the member functions of the CylDetLayer class.
*/
#include "include/CCylDetLayer.h"
#include "include/CCylSeg.h"

/*! \brief
 * Simple constructor pf the CylDetLayer class.

 * The straightforward constructor of the CylDetLayer class directly takes a vector of oriented cylindrical segments and a weight as input. We do not encourage its use. 
*/

CylDetLayer::CylDetLayer(std::vector<CylSeg> Seglist,double wgh) { 
  CylSegList=Seglist;
  weight=wgh;
}

/*! \brief
 * Advanced constructor of the CylDetLayer class.

 * The 'advanced' constructor reads a list of coordinates (a vector of 2-dim. arrays) in the cylindrical plane {z,h}, together with the weighing factor. It then builds the convex polygonal envelope associated with the coordinates, automatically detecting the orientation of segments with respect to the IP.
*/
 
CylDetLayer::CylDetLayer(std::vector<std::array<double,2>> ptlist,double wgh) { 
 //  reset all y-values to positive.
    if(ptlist.size()>=1){
     for(int i=0; i<ptlist.size(); i++){
      if(ptlist[i][1]<0)ptlist[i][1]=-ptlist[i][1];
     }
    }
 //  check that the list contains three non-aligned points.
    bool nonalign=false;
    double detcomp=0.;
    if(ptlist.size()>=3){
     for(int i=0; i<ptlist.size()-2; i++){
      for(int j=i+1; j<ptlist.size()-1; j++){
       if(abs(ptlist[j][0]-ptlist[i][0])>1e-5 || abs(ptlist[j][1]-ptlist[i][1])>1e-5) {
        for(int k=j+1; k<ptlist.size(); k++){
         detcomp=abs((ptlist[j][1]-ptlist[i][1])*(ptlist[k][0]-ptlist[i][0])-(ptlist[j][0]-ptlist[i][0])*(ptlist[k][1]-ptlist[i][1]));
         if(detcomp>1e-10)nonalign=true;
         if(nonalign)break;
        }
       }
       if(nonalign)break;
      }
      if(nonalign)break;
     }
    }
 //  find the points with farthest and closest theta aperture.
      double thmax=0.,thmin=4.*atan(1.);
      int imax=0,imin=0;
     if(nonalign){
      for(int i=0; i<ptlist.size(); i++){
       if(abs(ptlist[i][0]*ptlist[i][0]+ptlist[i][1]*ptlist[i][1])>1e-10){
        double th=acos(ptlist[i][0]/sqrt(ptlist[i][0]*ptlist[i][0]+ptlist[i][1]*ptlist[i][1]));
        if(th>thmax){
         thmax=th;
         imax=i;
        }
        if(th==thmax && ptlist[i][0]*ptlist[i][0]+ptlist[i][1]*ptlist[i][1]>ptlist[imax][0]*ptlist[imax][0]+ptlist[imax][1]*ptlist[imax][1]){
         thmax=th;
         imax=i;
        }
        if(th<thmin){
         thmin=th;
         imin=i;
        }
        if(th==thmin && ptlist[i][0]*ptlist[i][0]+ptlist[i][1]*ptlist[i][1]<ptlist[imin][0]*ptlist[imin][0]+ptlist[imin][1]*ptlist[imin][1]){
         thmin=th;
         imin=i;
        }
       }
      }
      if(imax==imin){
       nonalign=false;
       std::cout << "Warning! Something is wrong with the CylDetLayer constructor." << std::endl;
      }
     }
 //  build the external convex envelope.
      std::vector<int> ordlist,ordlist2;
     if(nonalign){
      ordlist.push_back(imax);
      std::vector<int> adjpt;
      int istart=imax;
      int count=0;
      do{
      count=count+1;
      adjpt.clear();
      for(int i=0; i<ptlist.size(); i++){
       bool conv=true;
       if(i==istart || (abs(ptlist[istart][0]-ptlist[i][0])<1e-5 && abs(ptlist[istart][1]-ptlist[i][1])<1e-5))conv=false;
       for(int j=0; j<ptlist.size(); j++){
        if(!conv)break;
        if(j==istart || j==i)continue;
        detcomp=(ptlist[istart][0]-ptlist[i][0])*(ptlist[j][1]-ptlist[i][1])-(ptlist[istart][1]-ptlist[i][1])*(ptlist[j][0]-ptlist[i][0]);
        if(detcomp<-1e-10)conv=false;
        if(abs(detcomp)<1e-10 && (ptlist[istart][0]-ptlist[i][0])*(ptlist[istart][0]-ptlist[i][0])+(ptlist[istart][1]-ptlist[i][1])*(ptlist[istart][1]-ptlist[i][1])<(ptlist[istart][0]-ptlist[j][0])*(ptlist[istart][0]-ptlist[j][0])+(ptlist[istart][1]-ptlist[j][1])*(ptlist[istart][1]-ptlist[j][1]))conv=false; 
       }
       if(conv)adjpt.push_back(i);
      }
      if(adjpt.size()==1){
       ordlist.push_back(adjpt[0]);
       istart=adjpt[0];
      } else {
       std::cout << "Warning! Something is wrong with the CylDetLayer constructor." << std::endl;
      }
      } while(ordlist[ordlist.size()-1]!=imin && count<=ptlist.size());
      
      count=0;
      istart=imin;
      ordlist2.push_back(imin);
      do{
      count=count+1;
      adjpt.clear();
      for(int i=0; i<ptlist.size(); i++){
       bool conv=true;
       if(i==istart || (abs(ptlist[istart][0]-ptlist[i][0])<1e-5 && abs(ptlist[istart][1]-ptlist[i][1])<1e-5))conv=false;
       for(int j=0; j<ptlist.size(); j++){
        if(!conv)break;
        if(j==istart || j==i)continue;
        detcomp=(ptlist[istart][0]-ptlist[i][0])*(ptlist[j][1]-ptlist[i][1])-(ptlist[istart][1]-ptlist[i][1])*(ptlist[j][0]-ptlist[i][0]);
        if(detcomp<-1e-10)conv=false;
        if(abs(detcomp)<1e-10 && (ptlist[istart][0]-ptlist[i][0])*(ptlist[istart][0]-ptlist[i][0])+(ptlist[istart][1]-ptlist[i][1])*(ptlist[istart][1]-ptlist[i][1])<(ptlist[istart][0]-ptlist[j][0])*(ptlist[istart][0]-ptlist[j][0])+(ptlist[istart][1]-ptlist[j][1])*(ptlist[istart][1]-ptlist[j][1]))conv=false; 
       }
       if(conv)adjpt.push_back(i);
      }
      if(adjpt.size()==1){
       ordlist2.push_back(adjpt[0]);
       istart=adjpt[0];
      } else {
       std::cout << "Warning! Something is wrong with the CylDetLayer constructor." << std::endl;
      }
      } while(ordlist2[ordlist2.size()-1]!=imax && count<=ptlist.size());
     }
 // define the oriented cylindrical segments.
  CylSegList.clear();
  if(ordlist.size()>1){
   for(int i=0;i<ordlist.size()-1;i++){
    CylSeg idSeg(ptlist[ordlist[i]],ptlist[ordlist[i+1]],-1);
    CylSegList.push_back(idSeg);
   }
  } else {
    std::cout << "Warning! Something is wrong with the CylDetLayer constructor." << std::endl;
  }
  if(ordlist2.size()>1){
   for(int i=0;i<ordlist2.size()-1;i++){
    CylSeg idSeg(ptlist[ordlist2[i]],ptlist[ordlist2[i+1]],1);
    CylSegList.push_back(idSeg);
   }
  } else {
    std::cout << "Warning! Something is wrong with the CylDetLayer constructor." << std::endl;
  }
  weight=wgh;
}

/*! \brief
 * Decay probability function for the CylDetLayer class.

 * The inDetDec class function computes the weighed decay probability within the detector layer. 
   It simply sums decay probabilities from the listed cyl. segments and applies an overall weight.
*/
 
double CylDetLayer::inDetDec(double th,double leff) {
  double Pdec=0.;
  if(CylSegList.size()>0 && th>=0 && th<=4.*atan(1.) && leff>0){
   for(int i=0; i<CylSegList.size(); i++) {
    Pdec=Pdec+CylSegList[i].DecProb(th,leff);
   }
   Pdec=weight*Pdec;
  }
  return Pdec;
}
