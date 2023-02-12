#pragma once
#include <algorithm>
#include <map>
#include <string>
#include <vector>

#include "CalculusUtilities.hpp"
#include "FunctionProprieties.hpp"
#include "Parsers.hpp"
#include "Point.hpp"

typedef std::map<std::string, FunctionProperties> MapOfFunctions;

//this class implements an Integrator. It is an object that contains the function name and the
//integration parameter. It has two child classes, TrapzIntegrator and MCIntegrator, which overwrite
//the evaluate method. This class has a constructor that parses the input string to find all the
//integration parameters.
class Integrator {
 public:
  double parameter;  //width if trapz int, numofpoints if monte carlo

  size_t dim;

  std::string fname;

  MapOfFunctions * funcMap;

  std::vector<double> lower_bounds;

  std::vector<double> upper_bounds;

 public:
  Integrator(std::string &, MapOfFunctions *);

  virtual double evaluate() = 0;
};

//this is a child class of Integrator, which overwrite the evaluate method, implementing a
//numerical integration method using trapezoids
class TrapzIntegrator : public Integrator {
  double width;

 public:
  TrapzIntegrator(std::string & str, MapOfFunctions * map) :
      Integrator(str, map),
      width(parameter) {
    if (width <= 0) {
      std::cerr << "The width must be a positive number" << std::endl;
      throw(std::invalid_argument("wrong number of arguments"));
    }
  }

  virtual double evaluate();
};

//this is a child class of Integrator, which overwrites the evaluate method, implementing a
//numerical integration method using Monte Carlo
class MCIntegrator : public Integrator {
  size_t numPoints;

 public:
  MCIntegrator(std::string & str, MapOfFunctions * map) : Integrator(str, map) {
    if (parameter >= 0) {
      if (parameter < 1e+7) {
        numPoints = (size_t)parameter;
      }
      else {
        std::cout << "Warning: number of points is too large. To avoid very long or "
                     "infinity loops, we reduce the number of points to 10 Million"
                  << std::endl;
        numPoints = 1e+7;
      }
    }
    else {
      std::cerr << "The number of points for Monte Carlo Integration must be >= 0."
                << std::endl;
      throw(std::invalid_argument("wrong number of arguments"));
    }
  }

  virtual double evaluate();
};
