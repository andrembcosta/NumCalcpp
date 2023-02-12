#pragma once
#include <iostream>
#include <map>

#include "Utilities.hpp"
#include "expr.h"

typedef std::map<std::string, FunctionProperties> MapOfFunctions;

//this class represents expressions that are custom functions define by the user
//using the define command
class FunctionExpression : public OperationExpression {
  MapOfFunctions * funcMap;

 public:
  FunctionExpression(const std::string & id,
                     const std::vector<Expression *> & params,
                     MapOfFunctions * map) :
      OperationExpression(id, params),
      funcMap(map) {}

  virtual ~FunctionExpression() {}

  virtual double evaluate() const;
};
