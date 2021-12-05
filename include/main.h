#include <string>
#include <exception>

class myexception: public std::exception
{
  virtual const char* what() const throw()
  {
    return "Standard exception";
  }
} defaultEx;

int die(std::string output){
  std::cout << output << std::endl;
  throw defaultEx;
}
//empty file, required for successful compilation
