#include "include/cornerpoint/matrix.h"


Matrix::Matrix(int size)											//Initializer for the Matrix, just assignes memory for the matrix-vectors and then implements the unity matrix
{
	int i;
    matrixSize=size;
    currentMatrix.resize(size, std::vector<double>(size, 0.0));
    for(i=0;i<size;i++)
    {
    	currentMatrix[i][i]=1.0;
    }
}

double Matrix::getMatrixEntry(int i, int j)							//Read-Out a specific matrix entry
{
    return currentMatrix[i][j];
}

double Matrix::getMatrixSize()										//Read out the matrix size
{
    return matrixSize;
}

void Matrix::setMatrixEntry(int i, int j, double value)				//Setting a matrix value, checking if the space for the entry is there
{
    if(i<matrixSize && j<matrixSize)
    {
        currentMatrix[i][j]=value;
    }
    else
    {
			#ifdef ENABLE_DEBUG
            std::cout << "Error: Matrix is not big enough to insert a value at this position."<<std::endl;
			#endif
    }
}

void Matrix::setMatrixSize(int size)								//Setting the matrix size, maybe should include a warning, that info will be lost when the new size is lower than the current size
{
    matrixSize=size;
    currentMatrix.resize(size, std::vector<double>(size,0));
}

Matrix Matrix::rotMatrix (int size, std::string axis, double angle)	//Implementing Rotation matrices, only 2x2 to 4x4 rotation matrices are implemented so far, 4 dimensions are not Minkowski space
{
	Matrix matrix(size);
	for(int i=0;i<size;i++)
	{
		for(int j=0;j<size;j++)
		{
			matrix.setMatrixEntry(i,j,0.0);
		}
	}
    if(size==2)
    {
        matrix.setMatrixEntry(0,0,cos(angle));
        matrix.setMatrixEntry(1,1,cos(angle));
        matrix.setMatrixEntry(0,1,-sin(angle));
        matrix.setMatrixEntry(1,0,sin(angle));
    }
    else if(size==3)
    {
        if(axis=="x")
        {
            //unchanged axis
            matrix.setMatrixEntry(0,0,1.0);

            //actual rotation
            matrix.setMatrixEntry(1,1,cos(angle));
            matrix.setMatrixEntry(2,2,cos(angle));
            matrix.setMatrixEntry(1,2,-sin(angle));
            matrix.setMatrixEntry(2,1,sin(angle));
        }
        else if(axis=="y")
        {
            //unchanged axis
            matrix.setMatrixEntry(1,1,1.0);

            //actual rotation
            matrix.setMatrixEntry(0,0,cos(angle));
            matrix.setMatrixEntry(2,2,cos(angle));
            matrix.setMatrixEntry(0,2,-sin(angle));
            matrix.setMatrixEntry(2,0,sin(angle));
        }
        else if(axis=="z")
        {
            //unchanged axis
            matrix.setMatrixEntry(2,2,1.0);

            //actual rotation
            matrix.setMatrixEntry(0,0,cos(angle));
            matrix.setMatrixEntry(1,1,cos(angle));
            matrix.setMatrixEntry(0,1,-sin(angle));
            matrix.setMatrixEntry(1,0,sin(angle));
        }
        else
        {
			#ifdef ENABLE_DEBUG
            std::cout << "Error: Axis not chosen correctly.";
			#endif
        }
    }
    else if(size==4)
    {
        matrix.setMatrixEntry(0,0,1.0);
        if(axis=="x")
        {
            //unchanged axis
            matrix.setMatrixEntry(1,1,1.0);

            //actual rotation
            matrix.setMatrixEntry(2,2,cos(angle));
            matrix.setMatrixEntry(3,3,cos(angle));
            matrix.setMatrixEntry(2,3,-sin(angle));
            matrix.setMatrixEntry(3,2,sin(angle));
        }
        else if(axis=="y")
        {
            //unchanged axis
            matrix.setMatrixEntry(2,2,1.0);

            //actual rotation
            matrix.setMatrixEntry(1,1,cos(angle));
            matrix.setMatrixEntry(3,3,cos(angle));
            matrix.setMatrixEntry(1,3,-sin(angle));
            matrix.setMatrixEntry(3,1,sin(angle));
        }
        else if(axis=="z")
        {
            //unchanged axis
            matrix.setMatrixEntry(3,3,1.0);

            //actual rotation
            matrix.setMatrixEntry(1,1,cos(angle));
            matrix.setMatrixEntry(2,2,cos(angle));
            matrix.setMatrixEntry(1,2,-sin(angle));
            matrix.setMatrixEntry(2,1,sin(angle));
        }
        else
        {
			#ifdef ENABLE_DEBUG
            std::cout << "Error: Axis not chosen correctly.";
			#endif
        }
    }
	return matrix;
}

Matrix Matrix::matrixMultiplikation(int size, Matrix a,Matrix b)	//Multiplikation of two matrices. No chekc implemented, if we can even multiply these two matrices
{
    int j,k,l;
    Matrix matrix(size);
    double currentValue=0.0;
    for(j=0;j<size;j++)
    {
    	for(k=0;k<size;k++)
    	{
    		currentValue=0.0;
    		for(l=0;l<size;l++)
    		{
    			currentValue+=a.getMatrixEntry(j,l)*b.getMatrixEntry(l,k);
    		}
    		matrix.setMatrixEntry(j,k,currentValue);
    	}
    }

    #ifdef EXPLICIT_CALC
    for(j=0;j<i;j++)
    {
    	std::cout << "Neue Matrix: ";
    	for(k=0;k<i;k++)
    	{
    		std::cout << "    "  <<matrix.getMatrixEntry(j,k);
        }
    	std::cout << std::endl;
    }
    std::cout<<"______________________________________________________________________________________________ "<<std::endl;
    for(j=0;j<i;j++)
    {
    	std::cout << "Matrix a: ";
    	for(k=0;k<i;k++)
    	{
    		std::cout << "    "  <<a.getMatrixEntry(j,k);
    	}
    	std::cout << std::endl;
    }
    std::cout<<"______________________________________________________________________________________________ "<<std::endl;
    for(j=0;j<i;j++)
    {
    	std::cout << "Matrix b: ";
    	for(k=0;k<i;k++)
    	{
    		std::cout <<   "    "  << b.getMatrixEntry(j,k);
    	}
    	std::cout << std::endl;
    }
    std::cout<<"______________________________________________________________________________________________ "<<std::endl;
	#endif
    return matrix;
}

Matrix Matrix::calcMatrixOfMinors(Matrix matrix)					//The Matrix of minors is needed in calculating the inverse of a matrix A. Its entries are the determinants of the minor matrix in A.
{
    int size=matrix.getMatrixSize();
    Matrix result(size);
    for(int i=0;i<size;i++)
    {
        for(int j=0;j<size;j++)
        {
        	//To calculate the determinant of the minor Matrix we first need to initialize the minor Matrix, This is done with the following if-statements and for-loops
            Matrix currentMinor(size-1);
            for(int l=0;l<size;l++)
            {
                if(l<i)
                {
                    for(int k=0;k<size;k++)
                    {
                        if(k<j)
                        {
                        	currentMinor.setMatrixEntry(l,k,matrix.getMatrixEntry(l,k));
                        }
                        else if(k>j)
                        {
                        	currentMinor.setMatrixEntry(l,k-1,matrix.getMatrixEntry(l,k));
                        }
                    }
                }
                else if(l>i)
                {
                        for(int k=0;k<size;k++)
                    {
                        if(k<j)
                        {
                        	currentMinor.setMatrixEntry(l-1,k,matrix.getMatrixEntry(l,k));
                        }
                        else if(k>j)
                        {
                        	currentMinor.setMatrixEntry(l-1,k-1,matrix.getMatrixEntry(l,k));
                        }
                    }
                }
            }
                
            #ifdef EXPLICIT_CALC
            for(int l=0;l<size-1;l++)
            {
               for (int k = 0; k <size-1; k++)
               {
                   std::cout<<currentMinor.getMatrixEntry(l,k)<<"\t";
               }       
               std::cout<<std::endl;         
            }
            
            std::cout<<std::endl<<currentMinor.calcDeterminant()<<"\t"<<"(i,j)="<<i<<","<<j<<std::endl;
            std::cout<<"______________________________________________________________________________________________ "<<std::endl;
            #endif
            //Then we can calculate the determinante of the Minor matrix and enter it into the resultMatrix.
            result.setMatrixEntry(i,j,currentMinor.calcDeterminant());
        }
        #ifdef EXPLICIT_CALC
        std::cout<<std::endl;
        #endif
    }
    return result;
}

Matrix Matrix::calcMatrixOfCofactors(Matrix matrix)					//Puts a 'Checker'-Template ontop of a Matrix, i.e. +1,-1,+1 etc., is needed for the inverse-calculation
{
     int size=matrix.getMatrixSize();
     Matrix resultCofactors(size);
     for(int i=0;i<size;i++)
     {
        for(int j=0;j<size;j++)
        {
            if((i%2==0 && j%2==0) || (i%2!=0 && j%2!=0))
            {
                resultCofactors.setMatrixEntry(i,j,matrix.getMatrixEntry(i,j));
            }
            else
            {
                 resultCofactors.setMatrixEntry(i,j,-matrix.getMatrixEntry(i,j));
            }
        }
     }
     return resultCofactors;
}

Matrix Matrix::transpose(Matrix matrix)								//Transposes a Matrix
{
    int size=matrix.getMatrixSize();
    Matrix result(size);
    for(int i=0;i<size;i++)
    {
        for(int j=0;j<size;j++)
        {
            result.setMatrixEntry(j,i,matrix.getMatrixEntry(i,j));
        }
    }
    return result;
}

Matrix Matrix::calcInverse(Matrix matrix)							//Calculating the Inverse of a Matrix, if it is possible. If it is not possible one obtains the identity matrix
{
    int size=matrix.getMatrixSize();
    //First we calculate the determinant, to see if there even exists an inverse.
    double det=matrix.calcDeterminant();
    Matrix resultInverse(size);

    //If it is not possible, we just return the initialized matrix, which is the identity by default.
    if(det==0)
    {
    	return resultInverse;
    }



    //Calculating the Minor matrix is the first step.
    resultInverse=Matrix::calcMatrixOfMinors(matrix);
    #ifdef EXPLICIT_CALC
    std::cout << std::endl<<det<< std::endl;
    std::cout << "Minor: "<<std::endl;
    Matrix::readOutMatrix(resultInverse);
    std::cout<<"______________________________________________________________________________________________ "<<std::endl;
    #endif
    
    
    //Next we lay the 'Checkers'-Template on the matrix,
    resultInverse=Matrix::calcMatrixOfCofactors(resultInverse);
    #ifdef EXPLICIT_CALC
     std::cout << "Cofactor: "<<std::endl;
    Matrix::readOutMatrix(resultInverse);
    std::cout<<"______________________________________________________________________________________________ "<<std::endl;
     #endif
    
    
    //Then we need to transpose the matrix and divide each entry by the determinant
    resultInverse=Matrix::transpose(resultInverse);
    #ifdef EXPLICIT_CALC
    std::cout << "Transpose: "<<std::endl;
    Matrix::readOutMatrix(resultInverse);
    std::cout<<"______________________________________________________________________________________________ "<<std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    #endif

    for(int i=0;i<size;i++)
    {
        for(int j=0;j<size;j++)
        {
            resultInverse.setMatrixEntry(i,j,resultInverse.getMatrixEntry(i,j)/det);
            #ifdef EXPLICIT_CALC
            std::cout<<resultInverse.getMatrixEntry(i,j)/det<<"\t";
            #endif
        }
        #ifdef EXPLICIT_CALC
        std::cout<<std::endl;
        #endif
    }
    return resultInverse;
}

double Matrix::calcDeterminant()									//Calculation of the determinant. This is done recursively, so tread carefully here with the size of your matrices
{
	//The usual way to calculate the determinant: Calculating the determinant of the minor matrix to an entry and then multiply that entry with the determinant. Add or subtract the value from the determinant if it is an even/odd entry.
    if(matrixSize==1)
    {
        return currentMatrix[0][0];
    }
    else
    {
        double resultDet=0.0;
        for(int i=0;i<matrixSize;i++)
        {
        	//Initializing the minor matrix
            Matrix currentMinor(matrixSize-1);
            for(int j=0;j<matrixSize;j++)
            {
                if(j<i)
                {
                    for(int k=1;k<matrixSize;k++)
                    {
                    	currentMinor.setMatrixEntry(j,k-1,currentMatrix[j][k]);
                    }
                }
                else if(j>i)
                {
                    for(int k=1;k<matrixSize;k++)
                    {
                    	currentMinor.setMatrixEntry(j-1,k-1,currentMatrix[j][k]);
                    }
                }
            }
            //Calculating the contribution of the entry to the determinant
            if(i%2==0)
            {
                resultDet+=currentMatrix[i][0]*currentMinor.calcDeterminant();
            }
            else
            {
                resultDet-=currentMatrix[i][0]*currentMinor.calcDeterminant();
            }
        }
        return resultDet;
    }
    
}

double Matrix::calcTrace()											//Calculation of the trace of a Matrix
{
	double result=0.0;
	for(int i=0;i<matrixSize;i++)
	{
		result+=currentMatrix[i][i];
	}
	return result;
}

void Matrix::readOutMatrix(Matrix matrix)							//Writes the entries of a matrix into the console
{
    int size=matrix.getMatrixSize();
    for(int i=0;i<size;i++)
    {
        for(int j=0;j<size;j++)
        {
            std::cout<<matrix.getMatrixEntry(i,j)<<"\t";
        }
        std::cout<<std::endl;
    }
}




