#include "../include/Point.hpp"

//this is a constructor for a point given all properties it has.
Point::Point(double w,
             size_t d,
             std::vector<double> * l_bounds,
             std::vector<double> * u_bounds) :
    width(w),
    dim(d),
    upper_bounds(u_bounds),
    lower_bounds(l_bounds),
    volume(1) {
  for (size_t i = 0; i < dim; i++) {
    if ((*lower_bounds)[i] < (*upper_bounds)[i]) {
      coords.push_back((*lower_bounds)[i]);
    }
    else {      //if upper >= lower then the domain has no points and the integral is zero
      dim = 0;  //dim = 0 is an easy way to get a zero integral
    }
  }

  //now we find the volume of the largest box we can fit with corner at coords
  //in most cases it will be width^dim, but we may be close to a bound
  for (size_t i = 0; i < dim; i++) {
    //if we can fit width in this direction, multiply volume by width
    if (coords[i] + width < (*upper_bounds)[i] - 0.00000000001) {
      boxDimensions.push_back(width);
      volume *= width;
    }
    //if not, we are really close to the boundary, so, the max dimension in this coordinate is
    //upper_bound - coords[i]
    else {
      boxDimensions.push_back((*upper_bounds)[i] - coords[i]);
      volume *= ((*upper_bounds)[i] - coords[i]);
    }
  }
}

//this method traverses the grid of points
//given a point, the next point is obtained by adding width to the coordinate of lowest possible index
//(one such that the actual coordinate + width does not becomes greater than its upper_bound)
//and setting all the coordinates behind it (which will certainly be equal to their upper_bound by
//the design of this algorithm) to their lower_bound.
//when we get close to a boundary, we do some adjusting, by, instead of adding width, adding a
//quantity that makes the coordinate value equal to its upper_bound
//the method also takes care of compute the correct volume at the new point
bool Point::next() {
  //reset volume to 1
  volume = 1;
  for (size_t i = 0; i < dim; i++) {
    //check if we can add to coordinate i
    if (coords[i] < (*upper_bounds)[i] - 0.00000000001) {
      //if we can add, width, do it, and multiply volume by width
      if (coords[i] + width < (*upper_bounds)[i] - 0.00000000001) {
        coords[i] += width;
        boxDimensions[i] = std::min(width, (*upper_bounds)[i] - coords[i]);
      }
      //if not, we are really close to the boundary, so, multiply volume by the distance to bound and
      //set coords[i] to upper_bound
      else {
        boxDimensions[i] = 0;
        coords[i] = (*upper_bounds)[i];
      }
      //set all the coordinates behind to lower_bound and boxDim to initial value
      for (size_t j = 0; j < i; j++) {
        boxDimensions[j] = std::min(width, (*upper_bounds)[j] - (*lower_bounds)[j]);
        coords[j] = (*lower_bounds)[j];
      }
      //update volume
      for (size_t k = 0; k < dim; k++) {
        volume *= boxDimensions[k];
      }
      //return a bool true meaning that it was possible to advance
      return true;
    }
  }
  //if all the coordinates fail the initial if clause, that means we reached the upper limit of
  //each coordinate, so, we traversed the domain. In this case, returns false indicating that
  return false;
}
