#ifndef _MATRIX
#define _MATRIX

//As matrix.h is included in all other headers, we define the define the debug conditionals here.
//#define ENABLE_DEBUG
//#define ENABLE_DEBUG_SETUP
//#define EXPLICIT_CALC
//#define EXPLICIT_SORT
//#define VERT_ROT_TEST
//#define mAPP2
//#define PLAIN_TEST
#ifdef  PLAIN_TEST
    //#define CARTESIAN
    //#define ANGULAR_TEST_IDEALIZED
    #ifndef CARTESIAN
        #define POLAR
    #endif
	//#define RASTER_READOUT
    #define DETECTOR_HITS
    //#define PLAIN_TEST_ARRAY
#endif







#include <exception>
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <math.h>
#include <vector>




/* This class implements matrices into our mathematical framework. Only quadratic matrices are implemented here.
 * The size of the matrices is variable through the implementation of std::vectors. 
 */




class Matrix {
public:
	Matrix()=default;
	~Matrix() {};
	Matrix(int);


	//Returning individual properties of the Matrix
	double getMatrixEntry(int, int);
	std::vector<double> getMatrixRow(int);
	std::vector<double> getMatrixColumn(int);
	double getMatrixSize();

	//Setting new Values to the Matrix
	void setMatrixEntry(int, int, double);
	void setMatrixSize(int);


	static Matrix rotMatrix (int, std::string,double);	//Rotation Matrix in (int)-dimension [2-4 so far], around (string)-axis by angle (double)
	static Matrix matrixMultiplikation(int, Matrix,Matrix);	//Multiplying two Matrices with dimension (int)
	static Matrix calcInverse(Matrix);			//Calculating the Inverse of a Matrix, if it is possible. If it is not possible one obtains the identity matrix here
	static Matrix calcMatrixOfMinors(Matrix);		//The Matrix of minors is needed in calculating the inverse of a matrix A. Its entries are the determinants of the minor matrix in A.
	static Matrix calcMatrixOfCofactors(Matrix);		//Puts a 'Checker'-Template ontop of a Matrix, i.e. +1,-1,+1 etc., is needed for the inverse-calculation
	static Matrix transpose(Matrix);			//Transposes a Matrix
	static void readOutMatrix(Matrix);			//Writes the entries of a matrix into the console
	double calcDeterminant();				//Calculation of the determinant. This is done recursively, so tread carefully here with the size of your matrices
	double calcTrace();					//Calculating the trace of a Matrix
	
	

private:
	std::vector<std::vector<double>> currentMatrix;		//This is the array used for the matrix. We use vectors, as these handle dynamic memory
								//allocation easier than when we implement it with arrays manually.
	int matrixSize=0;					//Size of the quadratic matrix.
};











#endif
