#pragma once
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "FunctionExpression.hpp"
#include "FunctionProprieties.hpp"
#include "Makers.hpp"
#include "ParserUtilities.hpp"
#include "Utilities.hpp"
#include "expr.h"

typedef std::map<std::string, FunctionProperties> MapOfFunctions;

//This file declares all the Parsing functions, for more details, check Parsers.cpp

//This is used in the define commands
void parseFuncDeclaration(
    std::string &,
    std::map<std::string, std::pair<std::vector<std::string> *, std::string> > *);
//this is used in the test commands
double parseTestCall(std::string &);

//These two are used to transform strings to expression objects
Expression * parseOp(std::string::iterator &, MapOfFunctions *);
//This function will iterate over the expression string to generate the Expression object
Expression * parse(std::string::iterator &, MapOfFunctions *);
