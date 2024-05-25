#ifndef _MAIN
#define _MAIN

#include <exception>
#include <iostream>
#include <string>

class myexception : public std::exception {
  virtual const char* what() const throw() { return "Standard exception"; }
};

int die(std::string output);

#endif
