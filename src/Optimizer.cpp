#include "../include/Optimizer.hpp"

typedef std::map<std::string, FunctionProperties> MapOfFunctions;

double getNum(std::string::iterator & it);

std::vector<double> operator+(const std::vector<double> &, const std::vector<double> &);

std::vector<double> operator*(const std::vector<double> &, double);

//this constructor does the parsing necessary to perform an optimization search (gradient ascent
//or gradient descent), depending if the input command is min or max
Optimizer::Optimizer(std::string str, MapOfFunctions * map) : startPoint(), funcMap(map) {
  std::string::iterator it = str.begin();
  std::string temp(it, (it + 3));
  if (temp == "min") {
    gamma = -1;  //case min
  }
  else {
    gamma = 1;  //case max
  }
  it += 3;
  skipSpace(it);
  //use some of the parser utilities function to get the fname
  if (!buildValidId(fname, it)) {
    std::cerr << "invalid ID for function name" << std::endl;
    throw(std::invalid_argument("invalid ID for function name"));
  }
  //then, use the map to obtain function dims
  if (!isValidOp(fname, funcMap)) {
    std::cout << "This function has not been defined yet. Cannot optimize it."
              << std::endl;
    throw(std::invalid_argument(
        "This function has not been defined yet. Cannot perform integration."));
  }
  dim = funcMap->find(fname)->second.argsList->size();
  //check if the inputs match dims
  size_t count_args = 0;
  try {
    while (*it != '\0' && *it != '#') {
      //get the optimizer parameters
      if (count_args == 0) {
        double newgamma = getNum(it);
        if (newgamma <= 0) {
          std::cout << "parameter gamma must be positive" << std::endl;
          throw(std::invalid_argument("negative gamma"));
        }
        gamma = gamma * newgamma;
        count_args++;
        continue;
      }
      if (count_args == 1) {
        convergedDistance = getNum(it);
        if (convergedDistance <= 0) {
          std::cout << "the tolerance must be positive" << std::endl;
          throw(std::invalid_argument("negative tolerance"));
        }
        count_args++;
        continue;
      }
      if (count_args == dim + 2) {
        //get maxSteps
        double temp;  //use this to verify maxSteps is positive and avoid casting errors
        temp = getNum(it);
        if (temp <= 0) {
          std::cout << "the maximum number of steps must be positive" << std::endl;
          throw(std::invalid_argument("negative maxSteps"));
        }
        if (temp > SIZE_MAX) {
          maxSteps = SIZE_MAX;
        }
        else {
          maxSteps = (size_t)temp;
        }
        count_args++;
        continue;
      }
      startPoint.push_back(getNum(it));
      count_args++;
      continue;
    }
  }

  catch (std::invalid_argument) {
    std::cerr << "Cannot perform optimization" << std::endl;
    throw;
  }

  //check if the number of variables in the function match the number of bounds given
  if (dim != startPoint.size() || count_args != dim + 3) {
    std::cerr << "This is not a valid call to Optimizer. Verify the number of args"
              << std::endl;
    throw(std::invalid_argument("wrong number of arguments"));
  }
}

//this function computes the gradient of a function at a point p (given as a vector of doubles)
std::vector<double> Optimizer::computeGrad(const std::vector<double> & p) {
  double h =
      0.00001;  //numerical parameter used to approximate derivative by finite difference
  std::vector<double> grad;
  for (size_t i = 0; i < dim; i++) {
    //evaluate the function at the point p
    double functionRight = evalFunctionAtPoint(fname, p, h, dim, funcMap);
    //evaluate the function at the point p + h * e_i (e_i is the unit vector in the ith direction)
    double functionLeft = evalFunctionAtPoint(fname, p, h, i, funcMap);
    //build finite difference
    double dfdx = (functionLeft - functionRight) / h;
    grad.push_back(dfdx);
  }
  return grad;
}

//this function used computed grad to perform the gradient ascent or descent method.
//at this point, gamma is <0 for descent and >0 for ascent, so this function just uses
//gamma. It doesnt need to know if its doing ascent or descent.
void Optimizer::optimize() {
  double residual = 1e+7;  //a very large initial residual, please, dont set your
  //tolerance larger than this or you will get garbage results.
  std::vector<double> pcurr = startPoint;
  size_t nSteps = 0;
  //we can escape this loop if the residual is small, if nSteps reaches maxSteps or
  //if the residual gets too large (method is diverging)
  while (abs(residual) > convergedDistance && nSteps < maxSteps &&
         abs(residual) < 1e+10) {
    std::vector<double> grad = computeGrad(pcurr);
    residual = gamma * norm(grad);
    pcurr = pcurr + computeGrad(pcurr) * gamma;
    nSteps++;
  }
  if (nSteps == maxSteps) {
    std::cout
        << "Reached maxSteps. The gradient method for optimization did not converge. "
        << "maxSepts = " << maxSteps << std::endl;
    return;
  }
  if (abs(residual) >= 1e+10) {
    std::cout << "The residual is very large. The method is probably diverging."
              << std::endl;
    return;
  }

  //print max or min
  std::cout << "(" << pcurr[0];
  for (size_t i = 1; i < pcurr.size(); i++) {
    std::cout << ", ";
    std::cout << pcurr[i];
  }
  std::cout << ")" << std::endl;
}
