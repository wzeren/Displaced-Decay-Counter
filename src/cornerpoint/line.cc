#include "include/cornerpoint/line.h"


Line::Line(Vertex a,Vertex b)                                                               //Initializer
{
	//If both points are the same or in other words their difference is the origin, we cannot describe a line in 3 dimensions, so we make a distinction in the initializer
    if(!Vertex::subtractVertices(a,b).isNull())
    {
        support=a;
        orientation=Vertex::subtractVertices(b,a);
    }
    else
    {
        #ifdef ENABLE_DEBUG
        std::cout <<"Line could not be created. The two vertices are the same.";
        #endif
    }
}

bool Line::linesHit(Line a, Line b)                                                         //Test, if two lines hit
{
    
    /* Calculating, if two lines hit is pretty straightforward in mathematics,
     * but one can quickly divide by 0 and not get a good result here. To avoid
     * this, we check the entries themselves and a greater parameter (called
     * definiteness here) to not be zero. When we found a combination where this is 
     * the case, it is easy to determine algebraically, if the lines hit or not.
     */
     
    int i,j,k;                                  //integers for 'for-loops'
    double t=0.0,s=0.0;                         //parameters of line equation
    double definiteness=0.0;                    //definiteness is used, to check if we are dividing by 0 at some point
    
    //Copying the important vertices of the lines to work with them
    Vertex aOrientation=a.getOrientation();
    Vertex bOrientation=b.getOrientation();
    Vertex aSupport=a.getSupport();
    Vertex bSupport=b.getSupport();
    
    /* Here we iterate through all combinations in calculating a lineHit. i=0 means, that we first go through the-x entries
     * and combine it with j=1, the y-entries. If this does not work we go to j=2, the z-entries, and afterwards we would go to i=1 etc.
     * We stop this process, when the 'slope-entry' and the definiteness is not zero.
     */
    i=0;    
    if(aOrientation.getVecArray(i)!=0.0 && definiteness==0.0)
    {
        j=1;
        definiteness=bOrientation.getVecArray(j)-(bOrientation.getVecArray(i)*aOrientation.getVecArray(j))/aOrientation.getVecArray(i);
        if(definiteness==0.0)
        {
            j=2;
            definiteness=bOrientation.getVecArray(j)-(bOrientation.getVecArray(i)*aOrientation.getVecArray(j))/aOrientation.getVecArray(i);            
        }
        if(definiteness==0.0)
        {
            i++;
        }
    }
    else if(aOrientation.getVecArray(i)==0)
    {
        i++;
    }
    if(aOrientation.getVecArray(i)!=0.0 && definiteness==0.0)
    {
        j=0;
        definiteness=bOrientation.getVecArray(j)-(bOrientation.getVecArray(i)*aOrientation.getVecArray(j))/aOrientation.getVecArray(i);
        if(definiteness==0.0)
        {
            j=2;
            definiteness=bOrientation.getVecArray(j)-(bOrientation.getVecArray(i)*aOrientation.getVecArray(j))/aOrientation.getVecArray(i);
            
        }
        if(definiteness==0.0)
        {
            i++;
        }
    }
    else if(aOrientation.getVecArray(i)==0 )
    {
        i++;
    }
    if(aOrientation.getVecArray(i)!=0.0 && definiteness==0.0)
    {
        j=0;
        definiteness=bOrientation.getVecArray(j)-(bOrientation.getVecArray(i)*aOrientation.getVecArray(j))/aOrientation.getVecArray(i);
        if(definiteness==0.0)
        {
            j=1;
            definiteness=bOrientation.getVecArray(j)-(bOrientation.getVecArray(i)*aOrientation.getVecArray(j))/aOrientation.getVecArray(i);
            
        }
        if(definiteness==0.0)
        {
            i++;
        }
    }
    else if(aOrientation.getVecArray(i)==0)
    {
        i++;
    }
    
    //if there is no possible way to calculate the meeting point, there is a break condition here
    if(definiteness==0.0)
    {
		#ifdef ENABLE_DEBUG
        std::cout << "Error: Divided by 0 in calculating lineHit"<<std::endl;
		#endif
        return false;
    }
    
    //Calculating the parameters of both line equations at which the lines meet, here we could divide by 0, if we are not careful before
    s=(aSupport.getVecArray(j)-bSupport.getVecArray(j)+((bSupport.getVecArray(i)-aSupport.getVecArray(i))*aOrientation.getVecArray(j))/aOrientation.getVecArray(i))/definiteness;
    t=(bSupport.getVecArray(i)-aSupport.getVecArray(i)+s*bOrientation.getVecArray(i))/aOrientation.getVecArray(i);
    
    
    
    
    /* Next, we check for consistency. In 2 dimensions lines always hit, if they are not parallel, but in 3 dimensions they might miss.
     * The consistency-condition is from the third entry located at k, that was not used in the combination above. 
     * If the third entry for both lines at s/t respectively is the same, then the lines hit. Otherwise, they do not.
     */
    if((i==0 && j==1) || (i==1 && j==0))
    {
        k=2;
    }
    else if((i==0 && j==2 )|| (i==2 && j==0))
    {
        k=1;
    }
    else if((i==2 && j==1 )|| (i==1 && j==2))
    {
        k=0;
    }
    
    
	#ifdef EXPLICIT_CALC
    int m;
    
    std::cout << definitenes <<std::endl;
    std::cout << "Support A         Support B"<<std::endl;
    for(m=0;m<3;m++)
    {
        std::cout << aSupport.getVecArray(m)<< "        "<<bSupport.getVecArray(m)<<std::endl;
    }
    std::cout << std::endl;
    std::cout << "Orientation A         Orientation B"<<std::endl;
    for(m=0;m<3;m++)
    {
        std::cout << aOrientation.getVecArray(m)<< "        "<<bOrientation.getVecArray(m)<<std::endl;
    }
	#endif
    
    

    if((bSupport.getVecArray(k)+s*bOrientation.getVecArray(k))-(aSupport.getVecArray(k)+t*aOrientation.getVecArray(k))<1e-12)
    {
        return true;
    }
    else
    {
        return false;
    }
}

Vertex Line::linesHitPoint(Line a, Line b)                                                  //Vertex, where two lines hit
{
    
    //The first part is the same as in the boolean 'linesHit', as we need to integers i,j,k and the line parameters t,s
    int i=0,j=0;
    double t,s;
    double definitenes=0.0;
    Vertex aOrientation=a.getOrientation();
    Vertex bOrientation=b.getOrientation();
    Vertex aSupport=a.getSupport();
    Vertex bSupport=b.getSupport();
    t=0.0;
    s=0.0;
    if(aOrientation.getVecArray(i)!=0.0 && definitenes==0.0)
    {
        j=1;
        definitenes=bOrientation.getVecArray(j)-(bOrientation.getVecArray(i)*aOrientation.getVecArray(j))/aOrientation.getVecArray(i);
        if(definitenes==0.0)
        {
            j=2;
            definitenes=bOrientation.getVecArray(j)-(bOrientation.getVecArray(i)*aOrientation.getVecArray(j))/aOrientation.getVecArray(i);            
        }
        if(definitenes==0.0)
        {
            i++;
        }
    }
    else if(aOrientation.getVecArray(i)==0)
    {
        i++;
    }
    if(aOrientation.getVecArray(i)!=0.0 && definitenes==0.0)
    {
        j=0;
        definitenes=bOrientation.getVecArray(j)-(bOrientation.getVecArray(i)*aOrientation.getVecArray(j))/aOrientation.getVecArray(i);
        if(definitenes==0.0)
        {
            j=2;
            definitenes=bOrientation.getVecArray(j)-(bOrientation.getVecArray(i)*aOrientation.getVecArray(j))/aOrientation.getVecArray(i);
            
        }
        if(definitenes==0.0)
        {
            i++;
        }
    }
    else if(aOrientation.getVecArray(i)==0 )
    {
        i++;
    }
    if(aOrientation.getVecArray(i)!=0.0 && definitenes==0.0)
    {
        j=0;
        definitenes=bOrientation.getVecArray(j)-(bOrientation.getVecArray(i)*aOrientation.getVecArray(j))/aOrientation.getVecArray(i);
        if(definitenes==0.0)
        {
            j=1;
            definitenes=bOrientation.getVecArray(j)-(bOrientation.getVecArray(i)*aOrientation.getVecArray(j))/aOrientation.getVecArray(i);
            
        }
        if(definitenes==0.0)
        {
            i++;
        }
    }
    else if(aOrientation.getVecArray(i)==0)
    {
        i++;
    }
    
    if(definitenes==0.0)
    {
		#ifdef ENABLE_DEBUG
        std::cout << "Error: Divided by 0 in calculating lineHit"<<std::endl;
		#endif
    }
    
    
    s=(aSupport.getVecArray(j)-bSupport.getVecArray(j)+((bSupport.getVecArray(i)-aSupport.getVecArray(i))*aOrientation.getVecArray(j))/aOrientation.getVecArray(i))/definitenes;
    t=(bSupport.getVecArray(i)-aSupport.getVecArray(i)+s*bOrientation.getVecArray(i))/aOrientation.getVecArray(i);
    
    
    
    
    //Here is, where this function differs from the boolean. We now use the parameters, where the lines hit to initialize a new vertex and return it.
    
    double resultArray[3];
    int m;
    for(m=0;m<3;m++)
    {
        resultArray[m]=aSupport.getVecArray(m)+t*aOrientation.getVecArray(m);
    }
    
	#ifdef EXPLICIT_CALC
    std::cout<<"The point where the lines hit is:"<<std:endl;
    for(m=0;m<3;m++)
    {
    	std::cout<<"Entry "<<m<<" is "<<resultArray[m]<<std::endl;
    }
	#endif

    Vertex result (resultArray[0],resultArray[1],resultArray[2],"Cartesian");
    return result;
}


Vertex Line::getPointOnLine(double parameter)												//This function was important in the debug process, it serves as a line equation f(x)
{
    Vertex result(support.getX()+parameter*orientation.getX(),support.getY()+parameter*orientation.getY(),support.getZ()+parameter*orientation.getZ(),"Cartesian");
    return result;
}















