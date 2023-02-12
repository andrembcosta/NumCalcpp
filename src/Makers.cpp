#include "../include/Makers.hpp"
//this function makes custom function objects
Expression * makeExpr(const std::string & id,
                      const std::vector<Expression *> & params,
                      MapOfFunctions * funcMap) {
  //check if op is custom or basic math
  //use isValidOp to check
  if (isValidOp(id, funcMap) == 1) {
    return makeMathExpr(id, params);
  }
  if (isValidOp(id, funcMap) == 2) {
    if (params.size() == funcMap->find(id)->second.argsList->size()) {
      return new FunctionExpression(id, params, funcMap);
    }
    else {
      std::cerr << "wrong number of args for expression of type " << id << std::endl;
      return NULL;
    }
  }
  else {
    std::cerr << "Impossible operation string: " << id << "\n";
    abort();
  }
}
//this function make all the basic math type of expression objects
Expression * makeMathExpr(const std::string & id,
                          const std::vector<Expression *> & params) {
  if (id == "+") {
    if (params.size() == 2) {
      return new PlusExpression(params);
    }
    else {
      std::cerr << "wrong number of args for expression of type " << id << std::endl;
      return NULL;
    }
  }
  if (id == "-") {
    if (params.size() == 2) {
      return new MinusExpression(params);
    }
    else {
      std::cerr << "wrong number of args for expression of type " << id << std::endl;
      return NULL;
    }
  }
  if (id == "*") {
    if (params.size() == 2) {
      return new TimesExpression(params);
    }
    else {
      std::cerr << "wrong number of args for expression of type " << id << std::endl;
      return NULL;
    }
  }
  if (id == "/") {
    if (params.size() == 2) {
      return new DivExpression(params);
    }
    else {
      std::cerr << "wrong number of args for expression of type " << id << std::endl;
      return NULL;
    }
  }
  if (id == "%") {
    if (params.size() == 2) {
      return new ModExpression(params);
    }
    else {
      std::cerr << "wrong number of args for expression of type " << id << std::endl;
      return NULL;
    }
  }
  if (id == "^") {
    if (params.size() == 2) {
      return new PowExpression(params);
    }
    else {
      std::cerr << "wrong number of args for expression of type " << id << std::endl;
      return NULL;
    }
  }
  if (id == "logb") {
    if (params.size() == 2) {
      return new LogExpression(params);
    }
    else {
      std::cerr << "wrong number of args for expression of type " << id << std::endl;
      return NULL;
    }
  }
  if (id == "select") {
    if (params.size() == 3) {
      return new SelExpression(params);
    }
    else {
      std::cerr << "wrong number of args for expression of type " << id << std::endl;
      return NULL;
    }
  }
  if (id == "floor") {
    if (params.size() == 1) {
      return new FloorExpression(params);
    }
    else {
      std::cerr << "wrong number of args for expression of type " << id << std::endl;
      return NULL;
    }
  }
  if (id == "ceil") {
    if (params.size() == 1) {
      return new CeilExpression(params);
    }
    else {
      std::cerr << "wrong number of args for expression of type " << id << std::endl;
      return NULL;
    }
  }
  if (id == "round") {
    if (params.size() == 1) {
      return new RoundExpression(params);
    }
    else {
      std::cerr << "wrong number of args for expression of type " << id << std::endl;
      return NULL;
    }
  }

  std::cerr << "Impossible operation symbol: " << id << "\n";
  abort();
}
