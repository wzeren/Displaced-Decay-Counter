#include "include/cornerpoint/cubic_detector.h"


CubicDetector::CubicDetector(std::vector<Vertex> cornersFront,std::vector<Vertex> cornersBack,int basicSidesNumberOfCorners)		//Initializer
{
	//First we ready the std::vectors to be of the needed length
    amountOfSideCorners=basicSidesNumberOfCorners;
    amountOfSides=amountOfSideCorners+2;
    corners.resize(amountOfSideCorners*2);
    sides.resize(amountOfSides);
    
    //Next, we initialize the front and back plain
    Plain front(cornersFront,amountOfSideCorners);
    Plain back(cornersBack,amountOfSideCorners);
    sides[0]=front;
    sides[1]=back;
    
    //As the plain initializer sorts the Corners, we can then generate all the other side plains with this loops
    for(int i=0;i<amountOfSideCorners;i++)
    {
        std::vector<Vertex> placeHolderCorners(4);
        if(i==amountOfSideCorners-1)
        {
            placeHolderCorners[0]=front.getCorner(i);
            placeHolderCorners[1]=front.getCorner(0);
            placeHolderCorners[2]=back.getCorner(i);
            placeHolderCorners[3]=back.getCorner(0);
        }
        else
        {
            placeHolderCorners[0]=front.getCorner(i);
            placeHolderCorners[1]=front.getCorner(i+1);
            placeHolderCorners[2]=back.getCorner(i);
            placeHolderCorners[3]=back.getCorner(i+1);
        }


        Plain placeHolderPlain(placeHolderCorners,4);
        sides[i+2]=placeHolderPlain;
    }
    
    //Initializing the corners of the detector in its private variable
    for(int i=0;i<amountOfSideCorners;i++)
    {
        corners[i]=cornersFront[i];
        corners[amountOfSideCorners+i]=cornersBack[i];        
    }
    
    
    #ifdef ENABLE_DEBUG_SETUP
    std::cout<<"The Plain-Corners of the detector are:"<<std::endl;
    for(int i=0;i<amountOfSides;i++)
    {
        std::cout<<"Plain "<<i+1<<":     "<<std::endl<<std::endl;
        for(int k=0;k<3;k++)
        {
            std::cout<<"Entry "<<k+1<<":        ";
            for(int j=0;j<sides[i].getNumberOfCorners();j++)
            {
                std::cout<<sides[i].getCorner(j).getVecArray(k)<<"    | ";
            }
            std::cout<<std::endl;
        }     
        std::cout<<"--------------------------------------------------------------------------------------------------------"<<std::endl;       
    }
    #endif
    
    
    
    //Rudimently calculating the center of the detector
    std::vector<double> sumOfCenter(3,0.0);
    for(int i=0;i<amountOfSides;i++)
    {
        for(int j=0;j<3;j++)
        {
             sumOfCenter[j]+=sides[i].getCenter().getVecArray(j);
             if(i==amountOfSides-1)
             {
                 sumOfCenter[j]/=amountOfSideCorners;
             }
        }
    }
    centerOfDetector.update(sumOfCenter[0],sumOfCenter[1],sumOfCenter[2],"Cartesian");
    hitPlainIn=0;
    hitPlainOut=0;
}

bool CubicDetector::isDetectorHit(Line line)																						//Determining if the detector is hit. This also stores, which plane is hit for the event going in, and which plane it is going out as internal parameters
{
	//Basically we check if one side is hit and if this is the case, we look for the second plain hit.
	//Next we look at which hit-point is closer to the interaction point and based on that determine which side is hit first. Most of the time this will be the front plain
    int i,j;
    hitPlainIn=0;
    hitPlainOut=0;
    for(i=0;i<amountOfSides;i++)
    {
        if(sides[i].isAreaHit(line))
        {
			#ifdef ENABLE_DEBUG
        	std::cout<<"For the angles ("<<line.getOrientation().getTheta()*180/MY_PI<<","<<line.getOrientation().getPhi()*180/MY_PI<<") the first hit is for counter "<<i<<". ";
			#endif
           for(j=i+1;j<amountOfSides;j++)
           {
               if(sides[j].isAreaHit(line))
               {
					#ifdef ENABLE_DEBUG
            	    std::cout<<"The second plane hit is for counter "<<j<<"."<<std::endl<<"The lengths of these are (i,j) ("<<Vertex::vertexNorm(sides[i].hitPlainPoint(line))<<","<<Vertex::vertexNorm(sides[j].hitPlainPoint(line))<<")"<<std::endl<<std::endl;
					#endif
                   if(Vertex::vertexNorm(sides[i].hitPlainPoint(line))<Vertex::vertexNorm(sides[j].hitPlainPoint(line)))
                   {
                       hitPlainIn=i;
                       hitPlainOut=j;
					   #ifdef ENABLE_DEBUG
                       std::cout<<"_____________________________________________________________________________________________________________________________________________"<<std::endl;
						#endif
                       return true;
                   }
                   else if(Vertex::vertexNorm(sides[i].hitPlainPoint(line))>Vertex::vertexNorm(sides[j].hitPlainPoint(line)))
                   {
                       hitPlainIn=j;
                       hitPlainOut=i;
                       #ifdef ENABLE_DEBUG
                       std::cout<<"_____________________________________________________________________________________________________________________________________________"<<std::endl;
						#endif
                       return true;
                   }
                   else
                   {
                       #ifdef ENABLE_DEBUG
                       std::cout<<"It could not be determined, where the event entered/left the detector."<<"_____________________________________________________________________________________________________________________________________________"<<std::endl;
                       #endif
                       return false;
                   }                   
               }
               else if(j==amountOfSides-1)
               {
                    #ifdef ENABLE_DEBUG
                    std::cout<<"Event entered the detector, but did not leave it. Should never happen the way everything is implemented."<<"_____________________________________________________________________________________________________________________________________________"<<std::endl;
                    #endif
                    return false;
               }
           }
           return true;
        }
    }
    /*
    #ifdef ENABLE_DEBUG
    std::cout<<"Event did not hit the detector."<<std::endl;
    #endif*/
    return false;
}


double CubicDetector::lengthToDetector(Line line)																					//Calculating the length of an event until it hits the detector
{
    return Vertex::vertexLength(sides[hitPlainIn].hitPlainPoint(line));
}


double CubicDetector::lengthInDetector(Line line)																					//Calculating the length an event travels in the detector
{
    return Vertex::vertexLength(Vertex::subtractVertices(sides[hitPlainOut].hitPlainPoint(line),sides[hitPlainIn].hitPlainPoint(line)));
}
























