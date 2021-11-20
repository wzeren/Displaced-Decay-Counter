//#define ENABLE_DEBUG
//#define EXPLICIT_CALC


#include "include/cornerpoint/vertex.h"


Vertex::Vertex (double a,double b,double c, std::string coordinateSystemType)               //Initialiser of Vertice
{
  def=true;
  if(coordinateSystemType=="Cartesian")
  {
    //Initialising the values to correct variables according to Coordinate System Type
    vk_x=a;
    vk_y=b;
    vk_z=c;

    //Calculating the other values based on the initial Input

    vp_r=sqrt(pow(a,2)+pow(b,2)+pow(c,2));
    vp_theta=acos(c/vp_r);
    vp_phi=atan2(b,a);
    vc_r=sqrt(pow(a,2)+pow(b,2));
    vc_phi=atan2(b,a);
    vc_z=c;

    //To avoid redundancy we defined Phi from [0,2 Pi) and Theta to [0, pi). This needs to be corrected for in initialization, if an incorrect value was entered, or the atan2 function gave a phi<0. This is done here.
    while(vp_phi<0.0)
    {
    	vp_phi+=2*MY_PI;
    	vc_phi+=2*MY_PI;
    }
  }
  else if(coordinateSystemType=="Spherical")
  {

    //Check if the input respects the bounds, while initializing the values
    vp_r=a;
    vp_theta=std::fmod(1.0*b,MY_PI);
    vp_phi=std::fmod(1.0*c,2*MY_PI);

    //Handling redundancy again as in the Cartesian case. fmod from above handled multiplicity (n-pi, where n is an integer) and now we move the value into the correct frame
    while(vp_phi<0.0)
    {
    	vp_phi+=2*MY_PI;
    }
    while(vp_theta<0.0)
    {
    	vp_theta+=MY_PI;
    }
    //Calculating the other values based on the initial Input
    vk_x=a*sin(b)*cos(c);
    vk_y=a*sin(b)*sin(c);
    vk_z=a*cos(b);

    vc_r=a*sin(b);
    vc_phi=vp_phi;
    vc_z=vk_z;
  }
  else if(coordinateSystemType=="Cylindric")
  {
    //Initialising the values to correct variables according to Coordinate System Type
    vc_r=a;
    vc_phi=std::fmod(1.0*b,2*MY_PI);
    vc_z=c;

    //Redundancy of angles handled with while-loops and fmod-function
    while(vc_phi<0.0)
    {
    	vc_phi+=2*MY_PI;
    }

    //Calculating the other values based on the initial Input
    vk_x=a*cos(b);
    vk_y=a*sin(b);
    vk_z=c;

    vp_r=sqrt(pow(a,2.0)+pow(c,2.0));
    vp_theta=atan2(c,a);
    vp_phi=vc_phi;
    while(vp_theta<0.0)
    {
    	vp_theta+=MY_PI;
    }
  }
  else
  {
	  #ifdef ENABLE_DEBUG
	  std::cout << "Coordinate System is unknown.";
	  #endif
	  def=false;
  }
  vecArray[0]=vk_x;
  vecArray[1]=vk_y;
  vecArray[2]=vk_z;
}

void Vertex::update (double a,double b,double c, std::string coordinateSystemType)          //re-initialization of the Vertice, update its values
{
	def=true;
	  if(coordinateSystemType=="Cartesian")
	  {
	    //Initialising the values to correct variables according to Coordinate System Type
	    vk_x=a;
	    vk_y=b;
	    vk_z=c;

	    //Calculating the other values based on the initial Input

	    vp_r=sqrt(pow(a,2)+pow(b,2)+pow(c,2));
	    vp_theta=acos(c/vp_r);
	    vp_phi=atan2(b,a);
	    vc_r=sqrt(pow(a,2)+pow(b,2));
	    vc_phi=atan2(b,a);
	    vc_z=c;

	    //To avoid redundancy we defined Phi from [0,2 Pi) and Theta to [0, pi). This needs to be corrected for in initialization, if an incorrect value was entered, or the atan2 function gave a phi<0. This is done here.
	    while(vp_phi<0.0)
	    {
	    	vp_phi+=2*MY_PI;
	    	vc_phi+=2*MY_PI;
	    }
	  }
	  else if(coordinateSystemType=="Spherical")
	  {

	    //Check if the input respects the bounds, while initializing the values
	    vp_r=a;
	    vp_theta=std::fmod(1.0*b,MY_PI);
	    vp_phi=std::fmod(1.0*c,2*MY_PI);

	    //Handling redundancy again as in the Cartesian case. fmod from above handled multiplicity (n-pi, where n is an integer) and now we move the value into the correct frame
	    while(vp_phi<0.0)
	    {
	    	vp_phi+=2*MY_PI;
	    }
	    while(vp_theta<0.0)
	    {
	    	vp_theta+=MY_PI;
	    }
	    //Calculating the other values based on the initial Input
	    vk_x=a*sin(b)*cos(c);
	    vk_y=a*sin(b)*sin(c);
	    vk_z=a*cos(b);

	    vc_r=a*sin(b);
	    vc_phi=vp_phi;
	    vc_z=vk_z;
	  }
	  else if(coordinateSystemType=="Cylindric")
	  {
	    //Initialising the values to correct variables according to Coordinate System Type
	    vc_r=a;
	    vc_phi=std::fmod(1.0*b,2*MY_PI);
	    vc_z=c;

	    //Redundancy of angles handled with while-loops and fmod-function
	    while(vc_phi<0.0)
	    {
	    	vc_phi+=2*MY_PI;
	    }

	    //Calculating the other values based on the initial Input
	    vk_x=a*cos(b);
	    vk_y=a*sin(b);
	    vk_z=c;

	    vp_r=sqrt(pow(a,2.0)+pow(c,2.0));
	    vp_theta=atan2(c,a);
	    vp_phi=vc_phi;
	    while(vp_theta<0.0)
	    {
	    	vp_theta+=MY_PI;
	    }
	  }
	  else
	  {
		  #ifdef ENABLE_DEBUG
		  std::cout << "Coordinate System is unknown.";
		  #endif
		  def=false;
	  }
	  vecArray[0]=vk_x;
	  vecArray[1]=vk_y;
	  vecArray[2]=vk_z;
}

Vertex Vertex::addVertices(Vertex a,Vertex b)                                               //Addition of Vertices, done in Cartesian coordinates.
{
  double array[3];
  for(int i=0;i<3;i++)
  {
	array[i]=a.getVecArray(i)+b.getVecArray(i);
  }
  Vertex sum(array[0],array[1],array[2],"Cartesian");
  return sum;
}

Vertex Vertex::subtractVertices(Vertex a,Vertex b)                                          //Subtraction of Vertices, only done in Cartesian coordinates.
{
  double array[3];
  for(int i=0;i<3;i++)
  {
	array[i]=a.getVecArray(i)-b.getVecArray(i);
  }
  Vertex sum(array[0],array[1],array[2],"Cartesian");
  return sum;
}

double Vertex::scalarProduct(Vertex a,Vertex b)                                             //Scalar Product of Vertices
{
  double sum=0.0;
  for(int i=0;i<3;i++)
  {
	sum+=a.getVecArray(i)*b.getVecArray(i);
  }
  return sum;
}

double Vertex::vertexNorm(Vertex a)                                                         //Norm of a Vertex
{
    return Vertex::scalarProduct(a,a);
}

double Vertex::vertexLength(Vertex a)                                                       //Length of a Vertex
{
	return sqrt(Vertex::vertexNorm(a));
}

Vertex Vertex::crossProduct(Vertex one,Vertex two)                                          //Cross product between two Vertices
{
	double nx,ny,nz;

	nx=one.getY()*two.getZ()-one.getZ()*two.getY();
	ny=one.getZ()*two.getX()-one.getX()*two.getZ();
	nz=one.getX()*two.getY()-one.getY()*two.getX();

	Vertex normal(nx,ny,nz,"Cartesian");
	return normal;
}

Vertex Vertex::multMatrixVertex(Vertex vec,Matrix mat)                                      //Mulitplikation of Matrix and Vertex
{
        int k,j;
        double resultArray[3];
        
        for(k=0;k<3;k++)
        {
            resultArray[k]=0.0;
            for(j=0;j<3;j++)
            {
                    resultArray[k]+=mat.getMatrixEntry(k,j)*vec.getVecArray(j);
            }
        }
        Vertex result(resultArray[0],resultArray[1],resultArray[2],"Cartesian");
        return result;
}

bool Vertex::isNull()																		//Checking if the Vertex is the Origin
{
	if(vk_x==0.0 && vk_y==0.0 && vk_z==0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Vertex::readOutVecCart(Vertex vertex)													//Writing the Vec in the console in Cartesian Coordinates
{
    std::cout<<"(";
    for(int k=0;k<3;k++)
    {
        std::cout<<vertex.getVecArray(k);
        if(k<2)
        {
            std::cout<<"|";
        }
    }
    std::cout<<")";
}

void Vertex::readOutVecPol(Vertex vertex)													//Writing the Vec in the console in Spherical Coordinates
{
    std::cout<<"("<<vertex.getR()<<"|"<<vertex.getTheta()*180/MY_PI<<"|"<<vertex.getPhi()*180/MY_PI<<")";
}

