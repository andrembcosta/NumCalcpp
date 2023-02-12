#include "../include/FunctionExpression.hpp"

//just simple implementations of constructors
FunctionProperties::FunctionProperties(const std::vector<std::string> * arguments,
                                       const std::string & rule) :
    argsList(arguments),
    functionRule(rule),
    justInserted(true) {
}

FunctionProperties::FunctionProperties(const FunctionProperties & rhs) {
  functionRule = rhs.functionRule;
  argsList = rhs.argsList;
  justInserted = rhs.justInserted;
}

FunctionProperties & FunctionProperties::operator=(const FunctionProperties & rhs) {
  if (this != &rhs) {
    functionRule = rhs.functionRule;
    argsList = rhs.argsList;
    justInserted = rhs.justInserted;
  }
  return *this;
}

FunctionProperties::~FunctionProperties() {
}
