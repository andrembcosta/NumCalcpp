#include "../include/Utilities.hpp"

typedef std::map<std::string, FunctionProperties> MapOfFunctions;

//this function replaces all ocurrences of the string "from" (found in the string str) by the string "to"
std::string ReplaceAll(std::string str,
                       const std::string & from,
                       const std::string & to) {
  size_t start_pos = 0;
  while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos += to.length();
  }
  return str;
}

//this function skip spaces when parsing a string
void skipSpace(std::string::iterator & it) {
  while (isspace(*it)) {
    it++;
  }
}

//this function skip spaces in the beginning of a string
void skipSpaceString(std::string & str) {
  std::string::iterator it = str.begin();
  while (isspace(*it)) {
    it++;
  }
  str = std::string(it, str.end());
}
//I was using std::stod from c++11 to convert strings to doubles but I was told that
//I can't use c++11 unless I format all my code with c++11 style. So, I prefered to
//create my own stod using c++98 strtod
double myStod(const std::string & str) {
  const char * c_string = str.c_str();
  char * end;
  double ans = strtod(c_string, &end);
  if (c_string == end) {
    throw(std::invalid_argument("invalid string for conversion"));
  }
  return ans;
}

//I was using std::to_string to convert doubles to string but as explained before, I am
//avoiding using c++11 functions, so I created my own to_string function using c++98
std::string doubleToStr(double num) {
  std::ostringstream sstream;
  sstream << std::setprecision(17) << num;
  std::string ans = sstream.str();
  return ans;
}

//this function inserts all the basic math operations to the function map
void addBasicMath(MapOfFunctions * funcMap) {
  const std::vector<std::string> * argsVecBin = new std::vector<std::string>(2);
  //*argsVecBin = {"a","b"};
  FunctionProperties thisFuncProps(argsVecBin, "BasicMath2");

  //add binary basic math operations to the map
  funcMap->insert(std::pair<std::string, FunctionProperties>("+", thisFuncProps));
  funcMap->insert(std::pair<std::string, FunctionProperties>("-", thisFuncProps));
  funcMap->insert(std::pair<std::string, FunctionProperties>("*", thisFuncProps));
  funcMap->insert(std::pair<std::string, FunctionProperties>("/", thisFuncProps));
  funcMap->insert(std::pair<std::string, FunctionProperties>("%", thisFuncProps));
  funcMap->insert(std::pair<std::string, FunctionProperties>("^", thisFuncProps));
  funcMap->insert(std::pair<std::string, FunctionProperties>("logb", thisFuncProps));

  const std::vector<std::string> * argsVecUni = new std::vector<std::string>(1);
  FunctionProperties thisFuncProps1(argsVecUni, "BasicMath1");
  //add unary basic math operations to map
  funcMap->insert(std::pair<std::string, FunctionProperties>("floor", thisFuncProps1));
  funcMap->insert(std::pair<std::string, FunctionProperties>("ceil", thisFuncProps1));
  funcMap->insert(std::pair<std::string, FunctionProperties>("round", thisFuncProps1));

  const std::vector<std::string> * argsVecTri = new std::vector<std::string>(3);
  FunctionProperties thisFuncProps2(argsVecTri, "BasicMath3");
  //add ternary basic math operations to map
  funcMap->insert(std::pair<std::string, FunctionProperties>("select", thisFuncProps2));
}

//this function checks the function map to see if the function whose id is str
//has already been defined it returns 0 for false, 1 is the given function is a basic math
//operation and 2 if it is a custom function
int isValidOp(std::string str, MapOfFunctions * funcMap) {
  MapOfFunctions::iterator it = funcMap->find(str);
  if (it == funcMap->end()) {
    return 0;
  }
  else {
    if (it->second.functionRule == "BasicMath1" ||
        it->second.functionRule == "BasicMath2" ||
        it->second.functionRule == "BasicMath3") {
      return 1;
    }
    else {
      if (it->second.justInserted == true) {
        return 2;
      }
      return 2;
    }
  }
}

//this function frees all the memoty allocated in the map of functions
//it is used just before main returns
void freeMap(MapOfFunctions * functionMap) {
  MapOfFunctions::iterator it = functionMap->begin();
  //These bools are used to keep track of what BasicMath operations have been deleted
  //Since operations with the same number os args point to the same argsList vector,
  //we need something like this to avoid double freeing
  //as soon as a basic math operation is deleted, it makes its bool = true
  bool deletedUnaryOps = false;
  bool deletedBinaryOps = false;
  bool deletedTernaryOps = false;
  while (it != functionMap->end()) {
    //if unary math operation, delete if bool = false, else, do nothing, because it has been already deleted
    if (it->second.functionRule == "BasicMath1") {
      if (!deletedUnaryOps) {
        delete it->second.argsList;
        deletedUnaryOps = true;
        it++;
        continue;
      }
      else {
        it++;
        continue;
      }
    }
    //if binary math operation, delete if bool = false, else, do nothing, because it has been already deleted
    if (it->second.functionRule == "BasicMath2") {
      if (!deletedBinaryOps) {
        delete it->second.argsList;
        deletedBinaryOps = true;
        it++;
        continue;
      }
      else {
        it++;
        continue;
      }
    }
    //if ternary math operation, delete if bool = false, else, do nothing, because it has been already deleted
    if (it->second.functionRule == "BasicMath3") {
      if (!deletedTernaryOps) {
        delete it->second.argsList;
        deletedTernaryOps = true;
        it++;
        continue;
      }
      else {
        it++;
        continue;
      }
    }
    //if it was not a basic math operations, then it is a custom functions and must be deleted for sure
    delete it->second.argsList;
    it++;
  }
  //delete the map
  delete functionMap;
}
