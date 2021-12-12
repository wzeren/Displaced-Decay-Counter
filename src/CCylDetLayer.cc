#include "include/CCylDetLayer.h"
#include "include/CCylSeg.h"

CylDetLayer::CylDetLayer(std::vector<CylSeg> Seglist,double wgh) {  // direct constructor
  CylSegList=Seglist;
  weight=wgh;
}

CylDetLayer::CylDetLayer(std::vector<std::array<double,2>> ptlist,double wgh) {  // constructor from list of coordinates
 // reset all y-values to positive
    if(ptlist.size()>=1){
     for(int i=0; i<ptlist.size(); i++){
      if(ptlist[i][1]<0)ptlist[i][1]=-ptlist[i][1];
     }
    }
 // check that the list contains three non-aligned points
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
 // find the points with farthest and closest theta aperture
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
 // build the external convex envelope
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
 // define the segments
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

double CylDetLayer::inDetDec(double th,double leff) {  // sums decay probabilities from the listed cyl. segments
  double Pdec=0.;                                      // and applies an overall weight
  if(CylSegList.size()>0 && th>=0 && th<=4.*atan(1.) && leff>0){
   for(int i=0; i<CylSegList.size(); i++) {
    Pdec=Pdec+CylSegList[i].DecProb(th,leff);
   }
   Pdec=weight*Pdec;
  }
  return Pdec;
}

 /* function CylBrick creating standard bricks in order to fill a volume along the line
std::vector<CylDetLayer> LayerList;
LayerList.clear();
for(int zct=0; zct<zct0; zct++){
 for(int hct=0; hct<hct0; hct++){
  int count=0;
  double zcoord=dl/2.+zct*dl, hcoord=dh/2.+hct*dh;
  for(int phct=0; phct<phct0; phct++){
   double phcoor=phct*dphi;
   double xcoord=hcoord*cos(phcoor), ycoord=hcoord*sin(phcoord);   
   if(zcoord>=zmin && zcoord<=zmax && ycoord>=ymin && ycoord<=ymax && xcoord>=xmin && xcoord<=xmax)count=count+1;
  }
  if(count!=0){
   std::array<double,2> brkcoord={zcoord,hcoord};
   CylDetLayer newbrick=CylBrick(brkcoord,dl,dh,count*dphi,detwgh);
   LayerList.push_back(newbrick);
  }
 }
}
Detector myDetector(LayerList); */
