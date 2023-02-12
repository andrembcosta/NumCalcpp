#pragma once

#include <map>
#include <string>
#include <vector>

//this class is just a container for function's properties. It has a list of arguments, the function
//rule, which is a string and a bool that checks if the function has just been inserted in the map
//or if it has been inserted in previous steps
class FunctionProperties {
 public:
  const std::vector<std::string> * argsList;

  std::string functionRule;

  bool justInserted;

  FunctionProperties() {}

  FunctionProperties(const std::vector<std::string> *, const std::string &);

  FunctionProperties(const FunctionProperties &);

  FunctionProperties & operator=(const FunctionProperties &);

  ~FunctionProperties();
};
