#pragma once
#include <map>
#include <string>
#include <vector>

#include "FunctionProprieties.hpp"
#include "Parsers.hpp"
#include "Point.hpp"
#include "Utilities.hpp"

typedef std::map<std::string, FunctionProperties> MapOfFunctions;

//THIS FILE CONTAINS A LOT OF FUNCTIONS THAT ARE USED BY THE CALCULUS ROUTINES (INTEGRATOR AND
//OPTIMIZER)

//this function gets a string iterator, that should be pointing to a number, advances it to the next
//ending symbol (space, end of line or #) and converts the substring between the initial iterator and
//its final position to a double. If it finds some problems converting the string to double using myStod
//it throws a invalid_argument exception. Note that besides returning a double, it makes the iterator
//given point to the first char after the converted number
double getNum(std::string::iterator &);

//this function creates a vector with the digits of the binary representation of n
//with d digits
//example n=10 --> n = 1010 --> vector = {1,0,1,0} (considering d = 4)
std::vector<double> binVector(size_t, size_t);

//this function performs a vector sum, summing coordinate by coordinate
std::vector<double> operator+(const std::vector<double> &, const std::vector<double> &);

//this function performs a vector muliplication, coordinate by coordinate
std::vector<double> operator*(const std::vector<double> &, const std::vector<double> &);

//this function performs a multiplication of a vector and a scalar
std::vector<double> operator*(const std::vector<double> &, double);

//this function computes the norm of a vector of doubles
double norm(const std::vector<double> &);

//this functions evaluates a function fname at a point basePoint (if you set hcoord to dim) or
//at a neighbor point if you set hcoord to a number from 0 to dim-1. This neighboor is obtained by
//adding h to the hcoord entry of basePoint
double evalFunctionAtPoint(std::string,
                           const std::vector<double> &,
                           double,
                           size_t,
                           MapOfFunctions *);

//this function performs the integration in one single prism, whose leftmost vertex is Point.
double integrate(const Point &, const std::string &, MapOfFunctions *);

//this function generates a vector of random numbers between the bounds lower and upper
//and create a std::string by concatenating these random numbers
//with this is string, it becomes easy to make an Expression object
std::string randomPointGenerator(std::vector<double> *, std::vector<double> *);
