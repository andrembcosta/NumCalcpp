#include "../include/CalculusUtilities.hpp"

//this function gets a string iterator, that should be pointing to a number, advances it to the next
//ending symbol (space, end of line or #) and converts the substring between the initial iterator and
//its final position to a double. If it finds some problems converting the string to double using stod
//it throws a invalid_argument exception. Note that besides returning a double, it makes the iterator
//given point to the first char after the converted number
double getNum(std::string::iterator & it) {
  std::string::iterator auxIt =
      it;  //aux iterator, will be kept at initial location while it advances
  while (*it != ' ' && *it != '\0' && *it != '#') {
    it++;  //advance it until reaches end of current number
  }
  std::string strNum(auxIt, it);
  skipSpace(it);
  double ans;
  try {
    ans = myStod(strNum);
  }
  catch (std::invalid_argument) {
    std::cerr << "expected a number, but found " << strNum << std::endl;
    //return 0.0 / 0.0;
    throw;
  }
  if (std::isnan(ans)) {
    throw(std::invalid_argument("invalid string for conversion"));
  }
  return ans;
}

//this function creates a vector with the digits of the binary representation of n
//with d digits
//example n=10 --> n = 1010 --> vector = {1,0,1,0} (considering d = 4)
std::vector<double> binVector(size_t n, size_t d) {
  std::vector<double> ans;
  for (size_t i = 0; i < d; i++) {
    ans.insert(ans.begin(), n % 2);
    n = (n - (n % 2)) / 2;
  }
  return ans;
}

//this function performs a vector sum, summing coordinate by coordinate
std::vector<double> operator+(const std::vector<double> & a,
                              const std::vector<double> & b) {
  std::vector<double> result;
  for (size_t i = 0; i < a.size(); i++) {
    result.push_back(a[i] + b[i]);
  }
  return result;
}

//this function performs a vector muliplication, coordinate by coordinate
std::vector<double> operator*(const std::vector<double> & a,
                              const std::vector<double> & b) {
  std::vector<double> result;
  for (size_t i = 0; i < a.size(); i++) {
    result.push_back(a[i] * b[i]);
  }
  return result;
}

//this function performs a multiplication of a vector and a scalar
std::vector<double> operator*(const std::vector<double> & a, double b) {
  std::vector<double> result;
  for (size_t i = 0; i < a.size(); i++) {
    result.push_back(a[i] * b);
  }
  return result;
}

double norm(const std::vector<double> & v) {
  double ans = 0;
  if (v.size() == 0) {
    return 0;
  }
  else {
    for (size_t i = 0; i < v.size(); i++) {
      ans += v[i] * v[i];
    }
    return pow(ans, 0.5);
  }
}

//this functions evaluates a function fname at a point basePoint (if you set hcoord to dim) or
//at a neighbor point if you set hcoord to a number from 0 to dim-1. This neighboor is obtained by
//adding h to the hcoord entry of basePoint
double evalFunctionAtPoint(std::string fname,
                           const std::vector<double> & basePoint,
                           double h,
                           size_t hcoord,
                           MapOfFunctions * funcMap) {
  size_t dim = basePoint.size();
  //make expression to evaluate the function at the point p
  std::string toEval = "(" + fname;
  for (size_t j = 0; j < dim; j++) {
    toEval += " ";
    toEval += doubleToStr(basePoint[j] + (hcoord == j) * h);
  }
  toEval += ")";
  std::string::iterator it = toEval.begin();
  Expression * thisExp = parse(it, funcMap);
  double ans = thisExp->evaluate();
  delete thisExp;
  return ans;
}

//this function performs the integration in one single prism, whose leftmost vertex is Point.
double integrate(const Point & p, const std::string & fname, MapOfFunctions * funcMap) {
  double ans = 0;
  const std::vector<double> * coords = p.getCoords();
  //this loop can be a little trick to understand. Basically, the vertices in the base of the prism
  //are all given by the point p + width(in some of the entries. It is not difficult to see that there
  //is a bijection between points in the base and binary numbers of size dim. Just map each vertex with
  //the binary number that indicates which coordinates were added by width.
  //So, the loop will go over all number from 0 to 2^dim - 1, convert them to binary, put the entries in
  //a vector, use this vector and the point p to generate all the vertices in the base of the prism
  //then, compute the value of the function given by fname in each vertex and add to answer.
  for (size_t n = 0; n < pow(2, p.getDim()); n++) {
    std::vector<double> toAdd =
        *coords + binVector(n, p.getDim()) * (*p.getBoxDimensions());
    //make expression usign toAdd
    std::string toEval = "(" + fname;
    for (size_t i = 0; i < toAdd.size(); i++) {
      toEval += " ";
      toEval += doubleToStr(toAdd[i]);
    }
    toEval += ")";
    std::string::iterator it = toEval.begin();
    Expression * thisExp = parse(it, funcMap);
    ans = ans + thisExp->evaluate();
    delete thisExp;
  }
  return p.getVolume() * ans;
}

//this function generates a vector of random numbers between the bounds lower and upper
//and create a std::string by concatenating these random numbers
//with this is string, it becomes easy to make an Expression object
std::string randomPointGenerator(std::vector<double> * lower,
                                 std::vector<double> * upper) {
  std::string coordString =
      " ";  //empty string to store a sequence of double that will represent our points
  for (size_t i = 0; i < lower->size(); i++) {
    double randomNumber = (double)rand() / RAND_MAX;
    randomNumber = (*lower)[i] + randomNumber * ((*upper)[i] - (*lower)[i]);
    coordString += doubleToStr(randomNumber);
    coordString += " ";
  }
  coordString += ")";
  return coordString;
}
