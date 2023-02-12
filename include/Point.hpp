#pragma once
#include <algorithm>
#include <map>
#include <string>
#include <vector>

#include "FunctionProprieties.hpp"

typedef std::map<std::string, FunctionProperties> MapOfFunctions;

//this is class implements a "smart" point. It is basically a point that know the width of the
//integration and the bounds. It has a method next() that traverse the grids of points in the
//integration domain, and returns false when it is finished traversing
class Point {
  double width;

  size_t dim;

  std::vector<double> * upper_bounds;

  std::vector<double> * lower_bounds;

  std::vector<double> coords;

  std::vector<double> boxDimensions;  //this vector indicates the size of each side of the
  //integration box whose corner is the current point. Usually the box has all sides equal
  //to width, but if we are close to the edges, it may be different.

  double volume;  //this is the volume of the box whose lowest corner is given by coords
                  //just the product of all entries of boxDimensions

 public:
  //this constructor is useful for Integrator
  Point(double, size_t, std::vector<double> *, std::vector<double> *);

  //this default constructor is useful for Optimizer
  Point() : width(0), dim(0), upper_bounds(NULL), lower_bounds(NULL), coords() {}

  size_t getDim() const { return dim; }
  double getWidth() const { return width; }
  double getVolume() const { return volume; }
  const std::vector<double> * getCoords() const { return &coords; }
  const std::vector<double> * getBoxDimensions() const { return &boxDimensions; }

  bool next();
};
