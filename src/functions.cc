#include "include/functions.h"

// Translates a char* to a string
std::string charToString(char* x){
    std::ostringstream ss;
    ss << x;
    std::string s(ss.str());
    return s;
}
