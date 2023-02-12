#pragma once

#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "FunctionProprieties.hpp"

typedef std::map<std::string, FunctionProperties> MapOfFunctions;

//This file contains general functions that are useful throughout the program

//this replaces all ocurrences of a substring inside a string
std::string ReplaceAll(std::string, const std::string &, const std::string &);

//this skip spaces
void skipSpace(std::string::iterator &);

//this performs string to double conversion
double myStod(const std::string &);

//this performs double to string conversion
std::string doubleToStr(double);

//this is used in the beginning of the program to add all basic math operations to the
//function map
void addBasicMath(MapOfFunctions *);

//this function check the map and see if string fname is there
//it return 0 for false, 1 for basic math and 2 for custom function
int isValidOp(std::string, MapOfFunctions *);

//this function frees all the memory that holds the map of functions
void freeMap(MapOfFunctions *);
