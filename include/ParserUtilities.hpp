#pragma once
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "FunctionExpression.hpp"
#include "FunctionProprieties.hpp"
#include "Makers.hpp"
#include "Parsers.hpp"
#include "Utilities.hpp"
#include "expr.h"

typedef std::map<std::string, FunctionProperties> MapOfFunctions;

//The functions in this file help the functions in Parsers.hpp performing
//some tasks
bool buildValidId(std::string &, std::string::iterator &);

bool buildValidRhs(std::string &, std::string::iterator &);

bool testFunctionRule(const std::string &, MapOfFunctions *);
