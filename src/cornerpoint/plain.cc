#include "include/cornerpoint/plain.h"


Plain::Plain(const std::vector<Vertex> &planeCorners, int cornerNumbers)				//Initializing the Plain, most of the magic happens here, all of the important values are instantly calculated
{
	int i,j;
	//Checking, if we can even create a specific area, i.e. if we have at least 3 corners
    if(cornerNumbers<3)
    {
        std::cout<< "Plain  could not b created, to little vertices given to properly define it"<<std::endl;
        alignmentAxis='E';
    }
    else
    {
    	i=1;
    	//Calculating the variables, that define the infinite plain
        do
        {
            i++;
            support=planeCorners[0];
        	direction1=Vertex::subtractVertices(support,planeCorners[1]);
        	direction2=Vertex::subtractVertices(support,planeCorners[i]);
        	normal=Vertex::crossProduct(direction1,direction2);
			#ifdef ENABLE_DEBUG
        	Vertex::readOutVecCart(support);
        	std::cout<<"\t";
        	Vertex::readOutVecCart(direction1);
        	std::cout<<",";
        	Vertex::readOutVecCart(planeCorners[1]);
        	std::cout<<"||\t";
        	Vertex::readOutVecCart(direction2);
        	std::cout<<",";
        	Vertex::readOutVecCart(planeCorners[i]);
        	std::cout<<"||\t";
        	Vertex::readOutVecCart(normal);
        	std::cout<<"\n";
			#endif
            numberOfCorners=cornerNumbers;
            #ifdef ENABLE_DEBUG
            std::cout<<"Initializing: Try number is "<< i-1<<std::endl;
            #endif
        }
        while(normal.isNull() && i<cornerNumbers);
        
        //Checking if all points are in the plane
        bool allPointsInPlane=true;
        for(j=1;j<cornerNumbers;j++)                
        {
            #ifdef ENABLE_DEBUG
            std::cout<<"Point: "<< j<<"  "<<Vertex::scalarProduct(Vertex::subtractVertices(support,planeCorners[j]),normal) <<std::endl;
            #endif
            if(Vertex::scalarProduct(Vertex::subtractVertices(support,planeCorners[j]),normal)>0.1e-9)
            {
                allPointsInPlane=false;
            }
        }
        if(normal.isNull())
        {
            #ifdef ENABLE_DEBUG
        	std::cout <<"Plain could not be created with the given points."<<std::endl<<"The points were on a line."<<std::endl;
            #endif
            alignmentAxis='E';
        }
        else if(!allPointsInPlane)
        {
            #ifdef ENABLE_DEBUG
            std::cout <<"Plain could not be created with the given points."<<std::endl<<"Not all of the points were on the plane."<<std::endl;
            #endif
            alignmentAxis='E';
        }
        else
        {
			//Define the alignment and the angles for the rotated 2-d frame
			Plain::calcAlignment();
			Plain::calcAlignmentAngles();
			#ifdef ENABLE_DEBUG
			std::cout<< alignmentAxis<<std::endl;
			#endif

			//Initializing the vectors, that have the corners in them
			corners.resize(cornerNumbers);
			corners=planeCorners;
			rotatedCorners.resize(cornerNumbers);
            rotEdges.resize(cornerNumbers);
			for(i=0;i<cornerNumbers;i++)
			{
				rotatedCorners[i]=Plain::rotatedPlainVector(planeCorners[i]);
				#ifdef EXPLICIT_CALC
				for(j=0;j<3;j++)
				{
					std::cout<< "Corners before are "<<corners[i].getVecArray(j)<<"     and after rotation we have "<<rotatedCorners[i].getVecArray(j)<< std::endl;
				}
				std::cout <<std::endl;
				#endif
			}
            
            //Calculating a rough center of the plain. needed for the sorting of the corners
			double sum1=0.0,sum2=0.0;
			if(alignmentAxis=='z')
			{
				for(i=0;i<cornerNumbers;i++)
				{
					sum1+=rotatedCorners[i].getX();
					sum2+=rotatedCorners[i].getY();
				}
				sum1=sum1/cornerNumbers;
				sum2=sum2/cornerNumbers;
				rotatedCenter.update(sum1,sum2,rotatedCorners[0].getZ(),"Cartesian");
			}
			else if(alignmentAxis=='y')
			{
				for(i=0;i<cornerNumbers;i++)
				{
					sum1+=rotatedCorners[i].getX();
					sum2+=rotatedCorners[i].getZ();
				}
				sum1=sum1/cornerNumbers;
				sum2=sum2/cornerNumbers;
				rotatedCenter.update(sum1,rotatedCorners[0].getY(),sum2,"Cartesian");
			}
			else if(alignmentAxis=='x')
			{
				for(i=0;i<cornerNumbers;i++)
				{
					sum1+=rotatedCorners[i].getY();
					sum2+=rotatedCorners[i].getZ();
				}
				sum1=sum1/cornerNumbers;
				sum2=sum2/cornerNumbers;
				rotatedCenter.update(rotatedCorners[0].getX(),sum1,sum2,"Cartesian");
			}
			#ifdef ENABLE_DEBUG
            std::cout<<std::endl;
            std::cout<<"All is good before the Sort"<<std::endl;
            std::cout<<std::endl;
            #endif
            
            //Sorting the corners in the rotated frame starting at 12 o'clock
            Plain::sortCorners();
            
            #ifdef ENABLE_DEBUG
            std::cout<<std::endl;
            std::cout<<"All is good after the Sort"<<std::endl;
            std::cout<<std::endl;
            #endif
			for(i=0;i<cornerNumbers;i++)
			{
				corners[i]=Plain::rotatePlainVectorBack(rotatedCorners[i]);
			}

            //Initializing the vector for the edges in the rotated frame
			for(i=0;i<numberOfCorners;i++)
			{
				if(i==(numberOfCorners-1))
				{
					Line edge(rotatedCorners[i],rotatedCorners[0]);
					rotEdges[i]=edge;
				}
				else
				{
					Line edge(rotatedCorners[i],rotatedCorners[i+1]);
					rotEdges[i]=edge;
				}
			}

			//Calculating the other variables that define the specific area
			area=Plain::calculateArea(numberOfCorners);
			rotatedCenter=Plain::calculateCenter(numberOfCorners);
			center=Plain::rotatePlainVectorBack(rotatedCenter);
        }
    }
}

void Plain::calcAlignment()                                                             //Calculating which alignment we choose, usually z-axis, except when it is already aligned or z-value of the normal vector is 0;
{
	if(normal.getZ()==0.0)
		{
			if(normal.getY()==0.0)
			{
				if(normal.getX()==0.0)
				{
					preAligned=true;
					alignmentAxis='E';
					#ifdef ENABLE_DEBUG
					std::cout<< "Normal Vector is the Origin. Something went wrong in calculating alignment angles."<<std::endl;
					#endif
				}
				else
				{
					preAligned=true;
					alignmentAxis='x';
					#ifdef ENABLE_DEBUG
					std::cout<<"Plain is already aligned with the "<<alignmentAxis<<"-axis."<<std::endl;
					#endif
				}
			}
			else if(normal.getX()==0.0)
			{
				if(normal.getY()!=0.0)
				{
					preAligned=true;
					alignmentAxis='y';
					#ifdef ENABLE_DEBUG
					std::cout<<"Plain is already aligned with the "<<alignmentAxis<<"-axis."<<std::endl;
					#endif
				}
			}
			else
			{
				preAligned=false;
				alignmentAxis='x';
			}
		}
		else if(normal.getX()==0.0)
		{
			if(normal.getY()==0.0)
			{
				preAligned=true;
				alignmentAxis='z';
				#ifdef ENABLE_DEBUG
				std::cout<<"Plain is already aligned with the "<<alignmentAxis<<"-axis."<<std::endl;
				#endif
			}
			else
			{
				preAligned=false;
				alignmentAxis='z';
			}
		}
		else
		{
			preAligned=false;
			alignmentAxis='z';
		}
}

void Plain::calcAlignmentAngles()														//Getting the angles to aligned the plain with an axis
{
    double m1,m2;

    if(preAligned)									//If the plain is already aligned, we need no rotation, so the angles will be set to 0
    {
    	angle1=0.0;
    	angle2=0.0;
		#ifdef ENABLE_DEBUG
    	std::cout<<"Angles did not need to be calculated. No Rotation necessary"<<std::endl;
		#endif
    }
    else
    {
    	if(alignmentAxis=='x')
    	{
    		m1=-normal.getX()/normal.getY();
    		angle1=atan2(m1,1.0);
   	        angle2=0.0;                             // If the plain is not pre-aligned and the axis is 'x', then normal.getZ is 0 (see  calcAlignment). Thus this angle will automatically be 0.
   		}
   		else if(alignmentAxis=='z')
   		{
   			m1=normal.getX()/normal.getZ();
   	        angle1=atan2(m1,1.0);
   	        m2=normal.getY()/(normal.getZ()*(m1*sin(angle1)+cos(angle1)));
   	        angle2=atan2(m2,1.0);
   		}
   	    else
   	    {
			#ifdef ENABLE_DEBUG
   	        std::cout << "Alignment axis was not properly defined.";
			#endif
   	    }
    }
	#ifdef EXPLICIT_CALC
    std::cout<< "angle1: "<< angle1*180/MY_PI<< ",  angle2: "<< angle2*180/MY_PI<< std::endl;
    std::cout<< "m1: "<< m1<< ",  m2: "<< m2<< std::endl;
	#endif
}

Vertex Plain::rotatedPlainVector(Vertex vector)											//Rotating a Vector into the aligned-plain-frame
{
	#ifdef EXPLICIT_CALC
    std::cout<<angle1<<"\t"<<angle2<<std::endl;
	#endif
    if(!preAligned)
    {
   	    Matrix totalRotation(3);
   	    if(alignmentAxis=='z')
   	    {
            Matrix rot1(3);
            Matrix rot2(3);
   	    	rot1=Matrix::rotMatrix(3,"y",angle1);
   	    	rot2=Matrix::rotMatrix(3,"x",angle2);
            totalRotation=Matrix::matrixMultiplikation(3,rot1,rot2);
   	    }
   	    else if(alignmentAxis=='x')
   	    {
            Matrix rot1(3);
            Matrix rot2(3);
   	    	rot1=Matrix::rotMatrix(3,"z",angle1);
   	    	rot2=Matrix::rotMatrix(3,"y",angle2);
            totalRotation=Matrix::matrixMultiplikation(3,rot1,rot2);
   	    }
   	    Vertex rotVector;
   	    rotVector=Vertex::multMatrixVertex(vector,totalRotation);


		#ifdef EXPLICIT_CALC
   	    std::cout<< "angle1: "<< angle1<< ",  angle2: "<< angle2<< std::endl;
   	    std::cout<< "Der alte und der rotierte Normalvector betragen:"<<std::endl;
   	    for(int i=0;i<3;i++)
   	    {
   	        std::cout<< "Alt: " <<vector.getVecArray(i)<<",  Neu: "<< rotVector.getVecArray(i) <<std::endl;
   	    }
		#endif
        return rotVector;
    }
    else
    {
    	return vector;
    }
}

Vertex Plain::rotatePlainVectorBack(Vertex vector)										//Rotating back a vector
{
	if(!preAligned)
	{
		Matrix rot1(3);
		Matrix rot2(3);
		Matrix totalRotation(3);

		if(alignmentAxis=='z')
		{
			rot1=Matrix::rotMatrix(3,"y",angle1);
			rot2=Matrix::rotMatrix(3,"x",angle2);
		}
		else if(alignmentAxis=='x')
		{
			rot1=Matrix::rotMatrix(3,"z",angle1);
			rot2=Matrix::rotMatrix(3,"y",angle2);
	   	}
		totalRotation=Matrix::matrixMultiplikation(3,rot1,rot2);
		#ifdef ENABLE_DEBUG
		std::cout<<"Initial Rotation"<<std::endl;
		Matrix::readOutMatrix(totalRotation);
		#endif
		if(totalRotation.calcDeterminant()!=0.0)
		{
			#ifdef ENABLE_DEBUG
			std::cout<<"Is Matrix correctly inverse:"<<std::endl;
			Matrix::readOutMatrix(Matrix::matrixMultiplikation(3,totalRotation,Matrix::calcInverse(totalRotation)));
			std::cout<<"______________________________________________________________________________________________ "<<std::endl;
			#endif
			totalRotation=Matrix::calcInverse(totalRotation);
		}
		else
		{
			#ifdef ENABLE_DEBUG
			std::cout<<"Matrix is not invertible, when rotating back the vertices."<<std::endl;
			#endif
		}

   	    Vertex result;
   	    result=Vertex::multMatrixVertex(vector,totalRotation);
   	    return result;
	}
	else
	{
		return vector;
	}
}

Line Plain::rotatedLine(Line line)														//Rotating a line by the same angles as the plain, so we can work in the rotated frame
{
	 Line rotatedLine(Plain::rotatedPlainVector(line.getSupport()),Plain::rotatedPlainVector(Vertex::addVertices(line.getSupport(),line.getOrientation())));
	 return rotatedLine;
}

Line Plain::rotateLineBack(Line line)													//Undo the rotation of a line
{
	 Line unrotatedLine(Plain::rotatePlainVectorBack(line.getSupport()),Plain::rotatePlainVectorBack(Vertex::addVertices(line.getSupport(),line.getOrientation())));
	 return unrotatedLine;
}

bool Plain::isRight(Vertex a,Vertex b)													//Checking if a Vertex is to the right (clockwise) or not (counter-clockwise) to another Vertex and the center of the specific area/ it does not work if the difference crosses 12 o'clock.
{
    //This works only if the vertices are given in the rotated frame
	double xDiffA,xDiffB,yDiffA,yDiffB;
	if(alignmentAxis=='z')
	{
		xDiffA=a.getX()-rotatedCenter.getX();
		xDiffB=b.getX()-rotatedCenter.getX();

		yDiffA=a.getY()-rotatedCenter.getY();
		yDiffB=b.getY()-rotatedCenter.getY();
	}
	else if(alignmentAxis=='y')
	{
		xDiffA=a.getX()-rotatedCenter.getX();
		xDiffB=b.getX()-rotatedCenter.getX();

		yDiffA=a.getZ()-rotatedCenter.getZ();
		yDiffB=b.getZ()-rotatedCenter.getZ();
	}
	else if(alignmentAxis=='x')
	{
		xDiffA=a.getY()-rotatedCenter.getY();
		xDiffB=b.getY()-rotatedCenter.getY();

		yDiffA=a.getZ()-rotatedCenter.getZ();
		yDiffB=b.getZ()-rotatedCenter.getZ();
	}
	else
	{
		#ifdef ENABLE_DEBUG
		std::cout<<"Rotated Frame not properly defined, to determine order of Corners"<<std::endl;
		#endif
		return false;
	}
	if(xDiffA>=0.0 && xDiffB<0.0)
	{
		return true;
	}
	else if(xDiffA<0.0 && xDiffB>=0.0)
	{
		return false;
	}
	else if(xDiffA==0.0 && xDiffB==0.0)
	{
		if(yDiffA>=0.0 || yDiffB>=0.0)
		{
			return yDiffA-yDiffB>0.0;
		}
		return	yDiffB-yDiffA>0.0;
	}


	//Calculating the cross product of the two points, to determine the position
	double det=xDiffA*yDiffB-xDiffB*yDiffA;
	if(det<0)
	{
		return true;
	}
	else if(det>0)
	{
		return false;
	}

	//If we still have not determined relative position here, then the points are on the same line from the center
	//Then we determine the relative position by checking, which point is closer to the center
	double distanceA=xDiffA*xDiffA+yDiffA*yDiffA;
	double distanceB=xDiffB*xDiffB+yDiffB*yDiffB;
	return distanceA>distanceB;
}

bool Plain::isSorted(std::vector<Vertex> rotCornsIn)									//Checking if a Vertex-vector is sorted counter-clockwise
{
    //A vector of vertices is not sorted if one or both of the following condiditons are fulfilled:
    
    //One of the vertices is to the right of an earlier vertex.
    //The next to next vertex is closer to the current vertex than the next vertex.
    
    //The following algorithm checks for both
	int i;
	std::vector<Vertex> rotCornsCalc(numberOfCorners+3);
	rotCornsCalc[numberOfCorners+1]=rotCornsIn[0];
	rotCornsCalc[numberOfCorners+2]=rotCornsIn[1];
	for(i=0;i<=numberOfCorners;i++)
	{
		rotCornsCalc[i]=rotCornsIn[i];
	}
	for(i=0;i<numberOfCorners;i++)
	{
		if(Plain::isRight(rotCornsCalc[i],rotCornsCalc[i+1]))
		{
            #ifdef EXPLICIT_SORT
			std::cout<<"SORT-ERROR: Corners are not left of each other. Failed at step "<<i+1<<std::endl;
			#endif
			return false;
		}
        if(Vertex::vertexNorm(Vertex::subtractVertices(rotCornsCalc[i],rotCornsCalc[i+1]))>Vertex::vertexNorm(Vertex::subtractVertices(rotCornsCalc[i],rotCornsCalc[i+2])))
		{
            #ifdef EXPLICIT_SORT
			std::cout<<"SORT-ERROR: Another corner on the left side is closer to the Corner. Failed at step "<<i+1<<std::endl;
			#endif
			return false;
		}
	}
	return true;
}

void Plain::sortCorners()					                                           	//Sorting a Vertex-vector counter-clockwise
{
    #ifdef EXPLICIT_SORT
    std::cout<<"--------------------------------------------------------------------------------------------------------Start of the Sort"<<std::endl;
    std::cout<<"The simplistic center is given as"<<std::endl;
    std::cout<<std::endl;
    for(int j=0;j<3;j++)
    {
        std::cout<<"Entry "<<j+1<<" is "<<rotatedCenter.getVecArray(j)<<std::endl;
    }
    std::cout<<"--------------------------------------------------------------------------------------------------------"<<std::endl;
    #endif
	int i,breakCondition=0;                         // The break condition is implemented for the case, where the sorting algorithm does not function properly and gets stuck
	std::vector<Vertex> result(numberOfCorners+1);
    
    //Define a clock pointer pointing to 12 o'clock. This one will get pushed to the last place of the vector 'result' by the sorting algorithm
    Vertex placeholder;
    if(alignmentAxis=='z')
	{
		placeholder.update(rotatedCenter.getX(),rotatedCenter.getY()+10.0,rotatedCenter.getZ(),"Cartesian");
	}
	else if(alignmentAxis=='y')
	{
        placeholder.update(rotatedCenter.getX(),rotatedCenter.getY(),rotatedCenter.getZ()+10.0,"Cartesian");
	}
	else if(alignmentAxis=='x')
	{
        placeholder.update(rotatedCenter.getX(),rotatedCenter.getY(),rotatedCenter.getZ()+10.0,"Cartesian");
	}
	else
	{
		#ifdef ENABLE_DEBUG
		std::cout<<"Rotated Frame not properly defined, to determine order of Corners"<<std::endl;
		#endif
	}
    //Fill the result array/vector
    result[0]=placeholder;
	for(i=1;i<=numberOfCorners;i++)
	{
		result[i]=rotatedCorners[i-1];
	}
	
    //Do the actual sorting, as long as the array is not sorted
    while(!Plain::isSorted(result) && breakCondition<50)
	{
		for(i=0;i<numberOfCorners;i++)
		{
			#ifdef EXPLICIT_SORT
            std::cout<<"At Position "<<i+1<<","<<i+2<<" of the array:"<<std::endl;
			std::cout<<"The two vertice before are"<<std::endl;
			for(int j=0;j<3;j++)
			{
				std::cout<<"                                 Left Vertex, Entry "<<j+1<<" is "<<result[i].getVecArray(j)<<" , Right Vertex Entry "<<j+1<<" is "<<result[i+1].getVecArray(j)<<std::endl;
			}
			#endif
			if(isRight(result[i],result[i+1]))
			{
                //If two corners are in the wrong position relativ to each other, they will be flipped here.
				placeholder=result[i];
				result[i]=result[i+1];
				result[i+1]=placeholder;
                #ifdef EXPLICIT_SORT
                std::cout<<std::endl;
                std::cout<<"The two vertice afterwards are switched."<<std::endl;
                #endif
			}
            else
            {
                #ifdef EXPLICIT_SORT
                std::cout<<std::endl;
                std::cout<<"The two vertice afterwards are NOT switched."<<std::endl;
                #endif
            }
            #ifdef EXPLICIT_SORT
            std::cout<<"--------------------------------------------------------------------------------------------------------End of one Comparison"<<std::endl;
			#endif
		}
        breakCondition++;
        #ifdef EXPLICIT_SORT
        std::cout<<"The breakCondition is at "<<breakCondition<<std::endl;
        std::cout<<"////////////////////////////////////////////////////////////////////////////////////////////////////////End of one sort-check"<<std::endl;
        std::cout<<std::endl;
        #endif
	}
    
    //Return the sorted Corners into the private variable. the last entry of result[numberofCorners] is the 'clock-hand' and will be forgotten afterwards
    for(i=0;i<numberOfCorners;i++)
    {
        rotatedCorners[i]=result[i];
    }
}

double Plain::calculateArea(int numberOfCorners)										//Calculating the area of the specific area in the rotated frame/ to do it this way, the corners need to be sorted counter-clockwise
{
    int i;
    double area=0;

    
    std::vector<double> a_corners(numberOfCorners+1,0.0);
    std::vector<double> b_corners(numberOfCorners+1,0.0);
    
    if(alignmentAxis=='z')
    {
        a_corners[numberOfCorners]=rotatedCorners[0].getVecArray(0);
        b_corners[numberOfCorners]=rotatedCorners[0].getVecArray(1);
        for(i=0;i<numberOfCorners;i++)
        {
        	a_corners[i]=rotatedCorners[i].getVecArray(0);
        	b_corners[i]=rotatedCorners[i].getVecArray(1);
        }
   	}
    else if(alignmentAxis=='y')
    {
        a_corners[numberOfCorners]=rotatedCorners[0].getVecArray(0);
        b_corners[numberOfCorners]=rotatedCorners[0].getVecArray(2);
        for(i=0;i<numberOfCorners;i++)
        {
        	a_corners[i]=rotatedCorners[i].getVecArray(0);
        	b_corners[i]=rotatedCorners[i].getVecArray(2);
        }
   	}
   	else if(alignmentAxis=='x')
   	{
   	    a_corners[numberOfCorners]=rotatedCorners[0].getVecArray(1);
   	    b_corners[numberOfCorners]=rotatedCorners[0].getVecArray(2);
   	    for(i=0;i<numberOfCorners;i++)
   	    {
   	    	a_corners[i]=rotatedCorners[i].getVecArray(1);
   	    	b_corners[i]=rotatedCorners[i].getVecArray(2);
   	    }
   	}
   	else
   	{
		#ifdef ENABLE_DEBUG
   		std::cout<<"Rotated Frame not properly defined, to determine order of Corners"<<std::endl;
		#endif
   		return 1.0;
   	}
    
    for(i=0;i<numberOfCorners;i++)
    {
        area+=((a_corners[i]*b_corners[i+1]-a_corners[i+1]*b_corners[i]))/2.0;
		#ifdef EXPLICIT_CALC
        std::cout << "Area in step "<<i<<" is "<< area <<std::endl;
		#endif
    }
    return area;
}

Vertex Plain::calculateCenter(int numberOfCorners)										//Calculating the center vertex in the rotated frame/ to do it this way, the corners need to be sorted counter-clockwise
{
    int i;
    double area=0;
    double a_center=0;
    double b_center=0;
    
    std::vector<double> a_corners(numberOfCorners+1,0.0);
    std::vector<double> b_corners(numberOfCorners+1,0.0);
    
    if(alignmentAxis=='z')
    {
    	a_corners[numberOfCorners]=rotatedCorners[0].getVecArray(0);
    	b_corners[numberOfCorners]=rotatedCorners[0].getVecArray(1);
    	for(i=0;i<numberOfCorners;i++)
    	{
    		a_corners[i]=rotatedCorners[i].getVecArray(0);
           	b_corners[i]=rotatedCorners[i].getVecArray(1);
        }
    }
    else if(alignmentAxis=='y')
    {
    	a_corners[numberOfCorners]=rotatedCorners[0].getVecArray(0);
    	b_corners[numberOfCorners]=rotatedCorners[0].getVecArray(2);
    	for(i=0;i<numberOfCorners;i++)
    	{
    		a_corners[i]=rotatedCorners[i].getVecArray(0);
           	b_corners[i]=rotatedCorners[i].getVecArray(2);
    	}
    }
    else if(alignmentAxis=='x')
    {
    	a_corners[numberOfCorners]=rotatedCorners[0].getVecArray(1);
    	b_corners[numberOfCorners]=rotatedCorners[0].getVecArray(2);
    	for(i=0;i<numberOfCorners;i++)
    	{
    		a_corners[i]=rotatedCorners[i].getVecArray(1);
    		b_corners[i]=rotatedCorners[i].getVecArray(2);
    	}
    }
    else
    {
		#ifdef ENABLE_DEBUG
    	std::cout<<"Rotated Frame not properly defined, to determine order of Corners"<<std::endl;
		#endif
    }
    for(i=0;i<numberOfCorners;i++)
    {
        area+=((a_corners[i]*b_corners[i+1]-a_corners[i+1]*b_corners[i]))/2.0;
    }
    
    for(i=0;i<numberOfCorners;i++)
    {
        a_center+=((a_corners[i]+a_corners[i+1])*(a_corners[i]*b_corners[i+1]-a_corners[i+1]*b_corners[i]))/(6*area);
        b_center+=((b_corners[i]+b_corners[i+1])*(a_corners[i]*b_corners[i+1]-a_corners[i+1]*b_corners[i]))/(6*area);
    }
    Vertex centerInRotation(0.0,0.0,0.0,"Cartesian");
    if(alignmentAxis=='z')
    {
    	centerInRotation.update(a_center,b_center,rotatedCorners[0].getVecArray(2),"Cartesian");
    }
    else if(alignmentAxis=='y')
    {
    	centerInRotation.update(a_center,rotatedCorners[0].getVecArray(1),b_center,"Cartesian");
    }
    else if(alignmentAxis=='x')
    {
    	centerInRotation.update(rotatedCorners[0].getVecArray(0),a_center,b_center,"Cartesian");
    }
	#ifdef EXPLICIT_CALC
    std::cout <<"The center in the rotated frame is "<<std::endl;
    for(i=0;i<3;i++)
    {
    	std::cout<<"Entry "<<i+1<<" is "<<centerInRotation.getVecArray(i)<<std::endl;
    }
	#endif

    return centerInRotation;
}

bool Plain::hitPlain(Line line)															//Checking if the plain is hit by a line or if the line is parallel to the plain
{
    Line rotatedLine=Plain::rotatedLine(line);

    //Checking which axis the plain is aligned to
    int i=0;
    if(alignmentAxis=='z')
    {
    	i=2;
    }
    else if(alignmentAxis=='y')
    {
    	i=1;
    }
    else if(alignmentAxis=='x')
    {
    	i=0;
    }
    if(rotatedLine.getOrientation().getVecArray(i)==0.0)
    {
    	return false;
    }
    else
    {
    	return true;
    }
}

Vertex Plain::hitRotPlainPoint(Line line)												//Vertex, where a plain is hit by a plain. We already presume here, that the plain is hit. Gives it in the rotated frame
{
    Line rotatedLine=Plain::rotatedLine(line);

    //Checking which axis the plain is aligned to
    int i,j,k;
    if(alignmentAxis=='z')
    {
    	i=2;
    	j=0;
    	k=1;
    }
    else if(alignmentAxis=='y')
    {
    	i=1;
    	j=0;
    	k=2;
    }
    else if(alignmentAxis=='x')
    {
    	i=0;
    	j=1;
    	k=2;
    }


    //Calculating the coordinates, where the plain is hit
    double parameter;
    parameter=(rotatedCenter.getVecArray(i)-rotatedLine.getSupport().getVecArray(i))/rotatedLine.getOrientation().getVecArray(i);
    double aHit,bHit;
    aHit=rotatedLine.getSupport().getVecArray(j)+parameter*rotatedLine.getOrientation().getVecArray(j);
    bHit=rotatedLine.getSupport().getVecArray(k)+parameter*rotatedLine.getOrientation().getVecArray(k);


    //Give out the Vertex where it hits the plain in the rotated frame
    Vertex eventAtPlain;
    if(i==2)
    {
    	eventAtPlain.update(aHit,bHit,rotatedCenter.getVecArray(2),"Cartesian");
    }
    else if(i==1)
    {
    	eventAtPlain.update(aHit,rotatedCenter.getVecArray(1),bHit,"Cartesian");
    }
    else if(i==0)
    {
    	eventAtPlain.update(rotatedCenter.getVecArray(0),aHit,bHit,"Cartesian");
    }
    return eventAtPlain;
}

Vertex Plain::hitPlainPoint(Line line)													//Vertex, where a plain is hit by a plain in unrotated frame.
{
	return rotatePlainVectorBack(Plain::hitRotPlainPoint(line));
}

bool Plain::isAreaHit(Line line)														//Check, whether or not a specific area is hit by a line
{
	//if plain is not hit at all we return false
	if(!hitPlain(line))
	{
		return false;
	}
    int i;
    Line rotatedLine=Plain::rotatedLine(line);											//rotateLine into the the aligned frame
    Vertex eventHitPoint=Plain::hitRotPlainPoint(line);									//obtaining the point, where the infinite plain is hit by the line.
    Line eventCenterLine(eventHitPoint,rotatedCenter);

    /* This algorithm is supposed to work the following way
     * 
     * It is easy to determine the point, where a line hits the infinite plain. With this point we can draw a line from the center of a plain to this hitPoint of the event.
     * This line hits at least two of the edges of the Plain, but most likely it hits all lines somewhere (remember, that the lines extend infinitely beyond the corner points)
     * 
     * First, we determine these edgeHitPoints. 
     * Next, we look at which edgeHitPoint is simultaneously closest to the the eventHitPoint and the center of the plain.
     * Then we just need to see which of these two points is closer to the center:
     * If the edgeHitPoint is closer --> the eventHitPoint is outside of the designated area
     * If the eventHitPoint is closer --> the eventHitPoint is inside of the designated area
     */ 

    //Calculating the shortest distance between an edge hit point and the event.
    std::vector<Vertex> edgeHitPoints(numberOfCorners);
    std::vector<double> distanceEdgeHit(numberOfCorners);
    std::vector<double> distanceEdgeCenter(numberOfCorners);
    std::vector<bool> edgeIsHit(numberOfCorners);
    
    
    //Here we first calculate all edgeHitPoints due to the event
    for(i=0;i<numberOfCorners;i++)
    {
        if(Line::linesHit(eventCenterLine,rotEdges[i]))
        {
            edgeIsHit[i]=true;
            edgeHitPoints[i]=Line::linesHitPoint(eventCenterLine,rotEdges[i]);
            distanceEdgeHit[i]=Vertex::vertexNorm(Vertex::subtractVertices(edgeHitPoints[i],eventHitPoint));
            distanceEdgeCenter[i]=Vertex::vertexNorm(Vertex::subtractVertices(edgeHitPoints[i],rotatedCenter));
        }
        else
        {
            edgeIsHit[i]=false;
            edgeHitPoints[i]=Vertex(0.0,0.0,0.0,"Cartesian");
            distanceEdgeHit[i]=0.0;
        }
    }
    
    //Next we determine the edgeHitPoint that is closest to the center and the eventHitPoint
    int shortestDistance=numberOfCorners;
    for(i=0;i<numberOfCorners;i++)
    {
    	if(edgeIsHit[i] && shortestDistance==numberOfCorners) //if there was no hit already, this is the new shortest distance
    	{
    		shortestDistance=i;
    	}
    	else if(edgeIsHit[i] && (distanceEdgeHit[i]+distanceEdgeCenter[i])<(distanceEdgeHit[shortestDistance]+distanceEdgeCenter[shortestDistance])) 
        {
            /* Here we check the sum of distance between edge-center and edge-event as to find the shortest distance to an edge and 
             * the shortest distance of this to a center. This ensures that it is simultaneously the shortest distance for both.
             */
    		shortestDistance=i;
        }
    }

	#ifdef ENABLE_DEBUG
    for(i=0;i<numberOfCorners;i++)
    {
    	if(edgeIsHit[i])
    	{
    		std::cout<<"1	";
    	}
    	else
    	{
    		std::cout<<"0	";
    	}
    }
    std::cout<<std::endl;
	#endif
    if(shortestDistance==numberOfCorners)
    {
        return false;
    }
    
    
    //Avoid redundancy in the angles: e.g. for phi=45 and phi=225 both lines might hit the plain (extend infinitely), but only one event really hits the detector
    //This assumes the interaction point to be the origin
    bool correctDirectionOfEvent=true;
    double lineParameter=-1.0;
    if(rotatedLine.getOrientation().getZ()!=0.0)
    {
        lineParameter=eventHitPoint.getZ()/rotatedLine.getOrientation().getZ();
    }
    else if(rotatedLine.getOrientation().getY()!=0.0)
    {
        lineParameter=eventHitPoint.getY()/rotatedLine.getOrientation().getY();
    }
    else if(rotatedLine.getOrientation().getX()!=0.0)
    {
        lineParameter=eventHitPoint.getY()/rotatedLine.getOrientation().getY();
    }
    if(lineParameter>0.0)
    {
        correctDirectionOfEvent=true;
    }
    else
    {
        correctDirectionOfEvent=false;
    }
    
    //Is the hit point inside or outside relative to the edge it is closest to, and is it the correct direction
    if(Vertex::vertexNorm(Vertex::subtractVertices(eventHitPoint,rotatedCenter))<Vertex::vertexNorm(Vertex::subtractVertices(rotatedCenter,edgeHitPoints[shortestDistance]))&& correctDirectionOfEvent)
    {
        return true;
    }
    else
    {
        return false;
    }
}




















