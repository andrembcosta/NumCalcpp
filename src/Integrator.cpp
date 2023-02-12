#include "../include/Integrator.hpp"

//this constructor basically build an Integrator object by parsing a call to numint or mcint
//and finding the parameters. It is responsible for finding a lot of problems with the input_line
Integrator::Integrator(std::string & str, MapOfFunctions * map) : funcMap(map) {
  std::string::iterator it = str.begin();
  skipSpace(it);
  //use some of the parser utilities function to get the fname
  if (!buildValidId(fname, it)) {
    std::cerr << "invalid ID for function name" << std::endl;  //handle error
    throw(std::invalid_argument("invalid ID for function name"));
  }
  //then, use the map to obtain function dims
  if (!isValidOp(fname, funcMap)) {
    std::cout << "This function has not been defined yet. Cannot perform integration."
              << std::endl;
    throw(std::invalid_argument(
        "This function has not been defined yet. Cannot perform integration."));
  }
  dim = funcMap->find(fname)->second.argsList->size();
  int count_args = 0;
  try {
    while (*it != '\0' && *it != '#') {
      //get the integration parameter
      if (count_args == 0) {
        parameter = getNum(it);
        count_args++;
        continue;
      }
      //get lower bounds
      if (count_args % 2 == 1) {
        lower_bounds.push_back(getNum(it));
        count_args++;
        continue;
      }
      //get upper bounds
      else {
        upper_bounds.push_back(getNum(it));
        count_args++;
        continue;
      }
    }
  }
  catch (std::invalid_argument) {
    std::cerr << "This call to Integrator could not be parsed, verify your syntax"
              << std::endl;
    throw;
  }
  //check if the number of variables in the function match the number of bounds given
  if (dim != upper_bounds.size() || dim != lower_bounds.size()) {
    std::cerr << "This is not a valid call to Integrator. Verify the number of args"
              << std::endl;
    throw(std::invalid_argument("wrong number of arguments"));
  }
}

//this function uses the function integrate and the class point to loop over all points in the lattice
//and compute the contributions of each prism to the total integral.
double TrapzIntegrator::evaluate() {
  //initialize a point to the beginning of the integration box
  Point p(width, dim, &lower_bounds, &upper_bounds);
  if (p.getDim() == 0) {  //in case upper bound < lower bound for some coordinate
    return 0;             //then the integral is zero
  }
  double integral = 0;
  //first point
  integral = integral + integrate(p, fname, funcMap) / pow(2, dim);
  while (p.next()) {
    //add the contrinuition of a box to the total integral
    integral = integral + integrate(p, fname, funcMap) / pow(2, dim);
  }
  return integral;
}

//this is the monte carlo method to compute integrals
double MCIntegrator::evaluate() {
  double integral = 0;
  //compute volume of base
  double volume = 1;
  for (size_t j = 0; j < lower_bounds.size(); j++) {
    if (upper_bounds[j] < lower_bounds[j]) {
      volume = 0;
    }
    else {
      volume = volume * (upper_bounds[j] - lower_bounds[j]);
    }
  }

  std::string preString = "(" + fname;
  for (size_t i = 0; i < numPoints; i++) {
    //make a string in the right form to be parsed with random points inside the bounds
    std::string toEval = preString + randomPointGenerator(&lower_bounds, &upper_bounds);
    std::string::iterator it = toEval.begin();
    //parse the string to make a expression object
    Expression * tempExp = parse(it, funcMap);
    //compute the contribution of this random point to the integral
    integral = integral + tempExp->evaluate() * volume / (double)numPoints;
    delete tempExp;
  }
  return integral;
}
