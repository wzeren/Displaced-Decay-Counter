#ifndef _MAIN
#define _MAIN


#include <iostream>
#include <string>
#include <exception>

class myexception: public std::exception
{
  virtual const char* what() const throw()
  {
    return "Standard exception";
  }
};

int die(std::string output);

#endif
