#pragma once
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "BasicMath.hpp"
#include "FunctionExpression.hpp"
#include "FunctionProprieties.hpp"
#include "Utilities.hpp"

typedef std::map<std::string, FunctionProperties> MapOfFunctions;

//this file contains 2 functions. One makes custom function objects and the other one
//make Basic Math (+, -, *, /, logb etc) objects

Expression * makeExpr(const std::string &,
                      const std::vector<Expression *> &,
                      MapOfFunctions *);

Expression * makeMathExpr(const std::string &, const std::vector<Expression *> &);
