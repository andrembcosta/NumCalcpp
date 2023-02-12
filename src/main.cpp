#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "../include/FunctionProprieties.hpp"
#include "../include/Integrator.hpp"
#include "../include/Optimizer.hpp"
#include "../include/Parsers.hpp"
#include "../include/Utilities.hpp"
#include "../include/expr.h"

typedef std::map<std::string, FunctionProperties> MapOfFunctions;

void parseFuncDeclaration(std::string &, MapOfFunctions *);

void skipSpaceString(std::string &);

void addBasicMath(MapOfFunctions *);

void freeMap(MapOfFunctions *);

int main() {
  MapOfFunctions * functionMap = new MapOfFunctions;
  //add basic operations to Map

  addBasicMath(functionMap);

  std::string input_line;
  while (std::getline(std::cin, input_line)) {
    //apply a skipline operation
    //If finds # or \0, continue(re enter while loop), else, go to next line
    skipSpaceString(input_line);
    if (input_line[0] == '\0' || input_line[0] == '#') {
      continue;
    }
    //if command is optimization (min, max) do this
    if (input_line.substr(0, 3) == "min" || input_line.substr(0, 3) == "max") {
      try {
        Optimizer thisOptimizer(input_line, functionMap);
        thisOptimizer.optimize();
        continue;
      }
      catch (std::invalid_argument) {
        std::cout << "main: Cannot perform optimization. Invalid commands " << std::endl;
        continue;
      }
    }

    //if command is trapezoidal integration, do this
    if (input_line.substr(0, 6) == "numint") {
      try {
        TrapzIntegrator thisIntegrator(input_line.erase(0, 6), functionMap);
        std::cout << "integral is " << thisIntegrator.evaluate() << std::endl;
        continue;
      }
      catch (std::invalid_argument) {
        std::cout << "main: Cannot perform integration. Invalid commands " << std::endl;
        continue;
      }
    }
    //if command is monte carlo integration, do this
    if (input_line.substr(0, 5) == "mcint") {
      try {
        MCIntegrator thisIntegrator(input_line.erase(0, 5), functionMap);
        std::cout << "integral is " << thisIntegrator.evaluate() << std::endl;
        continue;
      }
      catch (std::invalid_argument) {
        std::cout << "main: Cannot perform integration. Invalid commands " << std::endl;
        continue;
      }
    }

    //if command is define, do this
    if (input_line.substr(0, 6) == "define") {
      parseFuncDeclaration(input_line.erase(0, 6), functionMap);
      continue;
    }
    //if command is test, do this
    if (input_line.substr(0, 4) == "test") {
      input_line.erase(0, 4);
      double rhs;
      try {
        rhs = parseTestCall(input_line);
      }
      catch (std::invalid_argument & ex) {
        std::cerr << ex.what() << std::endl;
        continue;
      }
      std::string::iterator it = input_line.begin();
      Expression * thisLineExp = parse(it, functionMap);
      skipSpace(it);
      if (thisLineExp != NULL) {  //here we verify almost all other possible errors
        if (*it != '\0') {
          //here, we test for characters between the end of the expression and the last )
          //which is an error that the previous if statement doesnt catch
          std::cerr << "Unexpected character after expression given: " << *it
                    << std::endl;
          std::cerr << "Cannot evaluate it" << std::endl;
          delete thisLineExp;
          continue;
        }
        double correct_ans = thisLineExp->evaluate();
        thisLineExp->print();
        std::cout << " = " << rhs << " ";
        if (std::abs(correct_ans - rhs) < 0.00000000001) {
          std::cout << "[correct]" << std::endl;
        }
        else {
          std::cout << "[INCORRECT: expected " << correct_ans << "]" << std::endl;
        }
        delete thisLineExp;
      }
      continue;
    }
    else {
      std::cout << "Sorry, I don't understand this command" << std::endl;
    }
  }

  freeMap(functionMap);

  return EXIT_SUCCESS;
}
