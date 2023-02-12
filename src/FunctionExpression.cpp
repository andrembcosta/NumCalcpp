#include "../include/FunctionExpression.hpp"

#include "../include/Parsers.hpp"

typedef std::map<std::string, FunctionProperties> MapOfFunctions;

std::string doubleToStr(double);

//this function evaluate the expression to a numeric value and throws exceptions if that's not
//possible due to errors coming from parse or parseOp
double FunctionExpression::evaluate() const {
  //get function rule from map and use replace all to put in the values
  std::string ExpStr = funcMap->find(id)->second.functionRule;
  const std::vector<std::string> * args = funcMap->find(id)->second.argsList;
  //in this part, we substitute the variables in the functionRule to their actual numeric values
  //using the ReplaceAll function
  for (size_t i = 0; i < args->size(); i++) {
    std::string VarOut = ' ' + (*args)[i];
    std::string VarOut2 = VarOut + ')';
    VarOut.push_back(' ');
    double numValueIn = paramValues[i]->evaluate();
    std::string ValueIn = doubleToStr(numValueIn);
    ValueIn.insert(0, 1, ' ');
    std::string ValueIn2 = ValueIn + ')';
    ValueIn.push_back(' ');
    //we make 2 subs, the first handle the case where the variable is followed by a space, the second
    //the case where it is followed by a ). We need this because otherwise, if we had a variable x and
    //a variable xx, trying to substitue x by 1 would make xx = 11, which we dont want.
    ExpStr = ReplaceAll(ExpStr, VarOut, ValueIn);
    ExpStr = ReplaceAll(ExpStr, VarOut2, ValueIn2);
  }
  std::string::iterator ExpStrIt = ExpStr.begin();
  Expression * subExp = parse(ExpStrIt, funcMap);
  if (subExp == NULL) {
    delete subExp;
    throw std::invalid_argument("This function cannot be evaluated");
  }
  skipSpace(ExpStrIt);
  if (*ExpStrIt != '\0' && *ExpStrIt != '#') {
    delete subExp;
    throw std::length_error("Weird stuff after final )");
  }
  double ans = subExp->evaluate();
  delete subExp;
  return ans;
}
