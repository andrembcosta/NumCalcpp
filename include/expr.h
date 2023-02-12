#pragma once
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class Expression {
 public:
  virtual ~Expression() {}

  virtual void print() const = 0;

  virtual double evaluate() const = 0;
};

class OperationExpression : public Expression {
 protected:
  std::string id;

  std::vector<Expression *> paramValues;

 public:
  OperationExpression(const std::string & name, std::vector<Expression *> params) :
      id(name),
      paramValues(params) {}

  //we dont need copy constructor or assignment operator for these classes in this program
  //but that violates the rule of 3, so it is
  //safer to implement them with exceptions to avoid unintended use of the compiler's
  //default constructor in these cases
  OperationExpression(const OperationExpression &) {
    throw(std::runtime_error("You need to implement a copy constructor to do that"));
  }

  virtual OperationExpression & operator=(const OperationExpression &) {
    throw(std::runtime_error("You need to implement an assignment operator to do that"));
  }

  virtual ~OperationExpression() {
    for (size_t i = 0; i < paramValues.size(); i++) {
      delete paramValues[i];
    }
  }

  virtual void print() const {
    std::cout << id;
    std::cout << "(";
    for (size_t i = 0; i < paramValues.size(); i++) {
      paramValues[i]->print();
      if (i != paramValues.size() - 1) {
        std::cout << " ";
      }
    }
    std::cout << ")";
  }
};

class NumExpression : public Expression {
 private:
  double number;

 public:
  explicit NumExpression(double l) : number(l) {}

  virtual ~NumExpression() {}
  virtual double evaluate() const { return number; }

  virtual void print() const { std::cout << number; }
};
