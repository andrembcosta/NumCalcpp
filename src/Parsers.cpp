#include "../include/Parsers.hpp"

typedef std::map<std::string, FunctionProperties> MapOfFunctions;

void skipSpace(std::string::iterator &);

int isValidOp(std::string, MapOfFunctions *);

bool buildValidId(std::string &, std::string::iterator &);

bool buildValidRhs(std::string &, std::string::iterator &);

bool testFunctionRule(const std::string &, MapOfFunctions *);

void parseFuncDeclaration(std::string & declaration, MapOfFunctions * functionMap) {
  //This should read the function declaration
  //check if all ids are valid
  //start iterator it
  std::string::iterator it = declaration.begin();
  skipSpace(it);
  //This parts gets the function id
  std::string funcId;
  it++;
  skipSpace(it);
  if (buildValidId(funcId, it) == false) {
    std::cerr << "Cannot add this function" << std::endl;
    return;
  }
  //Lets check if it is not a repeated id
  if (functionMap->find(funcId) != functionMap->end()) {
    std::cerr << "Cannot add this function! Another function with the same id has "
                 "already been defined."
              << std::endl;
    return;
  }

  //now we continue iterating to get the args
  std::vector<std::string> * argsVector = new std::vector<std::string>;
  while (*it != ')') {
    std::string argId;
    if (buildValidId(argId, it) == false) {
      delete argsVector;
      std::cerr << "Cannot add this function" << std::endl;
      return;
    }
    //check for repeated args before adding
    if (std::find(argsVector->begin(), argsVector->end(), argId) != argsVector->end()) {
      std::cerr << "Repeated argument in function definition: " << argId << std::endl;
      std::cerr << "Cannot add this function" << std::endl;
      delete argsVector;
      return;
    }
    argsVector->push_back(argId);
  }
  //must have at least one arg
  if (argsVector->size() == 0) {
    std::cerr << "Functions need at least one argument" << std::endl;
    std::cerr << "Cannot add this function" << std::endl;
    delete argsVector;
    return;
  }
  //iterator is pointing at ')'
  it++;
  skipSpace(it);
  //now it should be pointing at '='
  if (*it != '=') {
    std::cerr << "expected a = sign, but found " << *it << std::endl;
    std::cerr << "Cannot add this function" << std::endl;
    delete argsVector;
    return;
  }
  it++;
  skipSpace(it);
  //now it should point to the '(' of the rhs of the declaration
  std::string rhsFormatted;
  if (buildValidRhs(rhsFormatted, it) == false) {
    std::cerr << "Cannot add this function" << std::endl;
    delete argsVector;
    return;
  }
  //now the iterator should be pointing to the end of the line
  //or to the beginning of a comment that will go to the end of the line
  //test if it is pointing to end of line or to #
  if (*it != '\0' && *it != '#') {
    std::cerr << "Cannot add this function" << std::endl;
    std::cerr << "expected a \0 or a #, but found a " << *it << std::endl;
    delete argsVector;
    return;
  }

  //If we are here, we passed all the tests, so, the function is valid and we can add it to the map
  //add function to map (key = funcId, value = pair<argsVector, rhsFormatted>)

  FunctionProperties thisFuncProps(argsVector, rhsFormatted);

  functionMap->insert(std::pair<std::string, FunctionProperties>(funcId, thisFuncProps));

  //We need to make sure that the function rule is valid. To do so, we call the newly created
  //function with all arguments set to zero. If eval returns NULL, then its not a valid function
  //testFunctionRule does all of this
  bool b = testFunctionRule(funcId, functionMap);
  if (!b) {
    std::cerr << "Cannot add this function" << std::endl;
  }
}

//this function is used when a "test" command is passed
//it will read the input line and extract the rhs, which is a double
//the input line will be modified, and, after a successful parseTestCall,
//it will be exactly the expression to be evaluated by parse.
//The rhs (which is a double) will be the return value
double parseTestCall(std::string & input_line) {
  //remove comments
  size_t endOfLine = input_line.find('#');
  input_line = input_line.substr(0, endOfLine);
  //reverse the string to parse from the end
  std::reverse(input_line.begin(), input_line.end());
  std::string::iterator it = input_line.begin();
  skipSpace(it);
  //create an iterator and advances it until the end of rhs
  //it can be ended in multiple chars ' ', ')', '\0'
  std::string::iterator auxIt = it;
  while (*it != ' ' && *it != ')' && *it != '\0') {
    it++;
  }
  std::string rhs(auxIt, it);
  std::reverse(rhs.begin(), rhs.end());
  std::string lhs(it, input_line.end());
  std::reverse(lhs.begin(), lhs.end());
  //redefine input_line, removing the RHS.
  input_line = lhs;

  double num;
  try {
    //try to convert rhs to double
    //num = std::stod(rhs);
    num = myStod(rhs);
  }
  catch (std::invalid_argument) {
    //catch stod errors
    if (rhs.empty()) {
      std::cerr << "Call to test with empty RHS." << std::endl;
      throw;
    }
    else {
      std::cerr << "Expected a number, but found " << rhs << std::endl;
      throw;
    }
  }
  //return the converted number
  return num;
}

//this function parses operations
//it is a helper to parse, which is defined next
//when parse finds an operation sign it call this function
//which will check if the operation is valid and create an expression object
//of the correct type
//it will call parse again to parse each argument of this expression object, since
//the arguments are expression themselves
Expression * parseOp(std::string::iterator & expStrIt, MapOfFunctions * funcMap) {
  std::string expressionId;
  //construct operatior id by looping over chars until finding a ' '
  while (*expStrIt != ' ') {
    skipSpace(expStrIt);
    expressionId.push_back(*expStrIt);
    expStrIt++;
  }
  //test if Id obtained corresponds to function in the map
  if (!isValidOp(expressionId, funcMap)) {
    std::cerr << "Invalid operation: " << expressionId << std::endl;
    return NULL;
  }

  //get correct number of params for specific function
  size_t numArgs = funcMap->find(expressionId)->second.argsList->size();
  //for i from 1 to size(map(id)_1), iterate over chars and parse them to build the numbers. If a '(' is
  //found, parse will call parseOp to handle it
  std::vector<Expression *> paramsVector;
  for (size_t i = 0; i < numArgs; i++) {
    paramsVector.push_back(parse(expStrIt, funcMap));
    //if pointer is NULL, then the expression is invalid and we have to free the allocated memory
    if (paramsVector.back() == NULL) {
      for (size_t j = 0; j < paramsVector.size(); j++) {
        delete paramsVector[j];
        return NULL;
      }
    }
  }

  skipSpace(expStrIt);

  if (*expStrIt == ')') {
    expStrIt++;
    return makeExpr(expressionId, paramsVector, funcMap);
  }

  std::cerr << "Expected ) but found " << *expStrIt << "\n";
  for (size_t j = 0; j < paramsVector.size(); j++) {
    delete paramsVector[j];
  }

  return NULL;
}
//This function will iterate over the expression string to generate the Expression object
//it calls parseOp when it finds an operation symbol
//there are 3 types of expressions objects, custom functions, basic math or numbers
//this function will create only numbers by itself and use parseOp to handle the other cases
Expression * parse(std::string::iterator & expStrIt, MapOfFunctions * funcMap) {
  skipSpace(expStrIt);
  if (*expStrIt == '\0' || *expStrIt == '#') {
    //one possible type of error
    std::cerr << "End of line or comment found in the midlle of expression!\n";
    return NULL;
  }
  else if (*expStrIt == '(') {
    // Found a '(', so, we call parseOp because the next char is part of an operator's name
    expStrIt++;
    return parseOp(expStrIt, funcMap);
  }
  else {
    //since this char is not a ( or an operation, it must be a number
    //we will use an auxiliar iterator to find the last digit of the number, then convert the string
    //between these iterators to double
    std::string::iterator auxIt = expStrIt;
    while (*expStrIt != ' ' && *expStrIt != ')') {
      expStrIt++;
    }
    std::string strNum(auxIt, expStrIt);
    try {
      double num = myStod(strNum);
      return new NumExpression(num);
    }
    catch (std::invalid_argument) {
      std::cerr << "Expected a number, but found " << strNum << "\n";
      return NULL;
    }
  }
}
