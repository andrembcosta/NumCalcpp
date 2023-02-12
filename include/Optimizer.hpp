#pragma once
#include <stdint.h>

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "CalculusUtilities.hpp"
#include "Parsers.hpp"

typedef std::map<std::string, FunctionProperties> MapOfFunctions;

//This class is responsible for optimization (find min and max) of functions
//for more info, check cpp file
class Optimizer {
 public:
  size_t dim;

  double gamma;

  double convergedDistance;

  size_t maxSteps;

  std::vector<double> startPoint;

  std::string fname;

  MapOfFunctions * funcMap;

 public:
  Optimizer(std::string, MapOfFunctions *);

  std::vector<double> computeGrad(const std::vector<double> &);

  void optimize();
};
