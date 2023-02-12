#include "../include/ParserUtilities.hpp"

typedef std::map<std::string, FunctionProperties> MapOfFunctions;

void skipSpace(std::string::iterator &);

int isValidOp(std::string, MapOfFunctions *);

//This function iterates over a string and build the first Id on it. Returns true if succeeds
//and false otherwise. Id is constructed in toBuild. Iterator will be modified to
//point to the first space after the Id in origin string, which is not an argument to this function.
//We only need the iterator
bool buildValidId(std::string & toBuild, std::string::iterator & iterator) {
  if (isalpha(*iterator) == 0) {
    std::cerr << "Invalid ID: First char is not a letter" << std::endl;
    return false;
  }
  while (*iterator != ' ' && *iterator != ')') {
    if (isalpha(*iterator) || isdigit(*iterator)) {
      toBuild.push_back(*iterator);
      iterator++;
    }
    else {
      std::cerr << "Invalid ID: contains invalid characters" << std::endl;
      return false;
    }
  }
  skipSpace(iterator);
  return true;
}

//This function iterates over the rhs of a function definition and remove some spaces
//that are necessary for the replace_all method to work when evaluting these expressions
bool buildValidRhs(std::string & toBuild, std::string::iterator & iterator) {
  //check if expression start with (
  if (*iterator != '(') {
    std::cerr << "Invalid RHS: Function expression should start with (, but found "
              << *iterator << std::endl;
    return false;
  }
  //start building rhs

  while (*iterator != '\0' && *iterator != '#') {
    if (*iterator == '(') {
      toBuild.push_back(*iterator);
      iterator++;
      //skip space after functions or operators
      skipSpace(iterator);
    }
    else {
      toBuild.push_back(*iterator);
      iterator++;
    }
  }
  toBuild.push_back('\0');
  return true;
}

//this function evalutate the function rule in the definition step, just to make sure it is a valid
//rule. It sets all parameters to zero and compute. If it is a valid functions (something that
//parse and parseOp can handle), the evaluation step will find some value (that can be NaN).
//If it is not a valid function, parse or parseOp will return NULL and the evaluation step
//will return an exception. Then this testFunctionRule will know that the function rule in not
//valid and delete the function from the map
bool testFunctionRule(const std::string & functionId, MapOfFunctions * functionMap) {
  //map iterator to current function
  MapOfFunctions::iterator it = functionMap->find(functionId);

  const std::string rule = it->second.functionRule;

  const std::vector<std::string> * argsVector = it->second.argsList;

  //make sure there are no recursive calls, otherwise we will have a stack overflow
  std::string toFind =
      "(" + functionId +
      " ";  //we have to construct toFind this way to avoid confusion between function names and args
  if (rule.find(toFind) != std::string::npos) {
    std::cerr << "Function definition cannot be recursive" << std::endl;
    delete argsVector;
    functionMap->erase(functionId);
    return false;
  }

  //make a string to test the function with all arguments set to zero
  std::string testInput = "(" + functionId;
  for (size_t i = 0; i < argsVector->size(); i++) {
    testInput += " 0";
  }
  testInput += ")";
  //parse the expression with all args set to zero
  std::string::iterator iter = testInput.begin();
  Expression * testNewFuncAtZeros = parse(iter, functionMap);
  int errorFlag = 0;
  try {
    testNewFuncAtZeros->evaluate();
  }
  catch (std::invalid_argument) {
    std::cerr << "This function's rule has problems" << std::endl;
    delete argsVector;
    delete testNewFuncAtZeros;
    functionMap->erase(functionId);
    errorFlag = 1;
    return false;
  }
  catch (std::length_error) {
    std::cerr << "This function contains extra characters after last )" << std::endl;
    delete argsVector;
    delete testNewFuncAtZeros;
    functionMap->erase(functionId);
    errorFlag = 1;
    return false;
  }

  delete testNewFuncAtZeros;
  if (!errorFlag) {
    functionMap->find(functionId)->second.justInserted = false;
    std::cout << "defined " << functionId << "(" << (*argsVector)[0];
    for (size_t i = 1; i < argsVector->size(); i++) {
      std::cout << ' ' << (*argsVector)[i];
    }
    std::cout << ")" << std::endl;
    return true;
  }
  return true;
}
