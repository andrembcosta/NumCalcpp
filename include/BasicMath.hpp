#pragma once
#include <math.h>

#include <string>

#include "expr.h"

class PlusExpression : public OperationExpression {
 public:
  explicit PlusExpression(std::vector<Expression *> params) :
      OperationExpression("+", params) {}

  virtual double evaluate() const {
    double ans = paramValues[0]->evaluate() + paramValues[1]->evaluate();
    return ans;
  }

  virtual ~PlusExpression() {}
};

class MinusExpression : public OperationExpression {
 public:
  explicit MinusExpression(std::vector<Expression *> params) :
      OperationExpression("-", params) {}

  virtual double evaluate() const {
    double ans = paramValues[0]->evaluate() - paramValues[1]->evaluate();
    return ans;
  }

  virtual ~MinusExpression() {}
};

class TimesExpression : public OperationExpression {
 public:
  explicit TimesExpression(std::vector<Expression *> params) :
      OperationExpression("*", params) {}

  virtual double evaluate() const {
    double ans = paramValues[0]->evaluate() * paramValues[1]->evaluate();
    return ans;
  }

  virtual ~TimesExpression() {}
};

class DivExpression : public OperationExpression {
 public:
  explicit DivExpression(std::vector<Expression *> params) :
      OperationExpression("/", params) {}

  virtual double evaluate() const {
    double ans = paramValues[0]->evaluate() / paramValues[1]->evaluate();
    return ans;
  }

  virtual ~DivExpression() {}
};

class ModExpression : public OperationExpression {
 public:
  explicit ModExpression(std::vector<Expression *> params) :
      OperationExpression("%", params) {}

  virtual double evaluate() const {
    long base = lround(paramValues[1]->evaluate());
    if (base != 0) {
      long ans = lround(paramValues[0]->evaluate()) % base;
      return (double)ans;
    }
    else {
      return 0.0 / 0.0;
    }
  }

  virtual ~ModExpression() {}
};

class PowExpression : public OperationExpression {
 public:
  explicit PowExpression(std::vector<Expression *> params) :
      OperationExpression("^", params) {}

  virtual double evaluate() const {
    double ans = pow(paramValues[0]->evaluate(), paramValues[1]->evaluate());
    return ans;
  }

  virtual ~PowExpression() {}
};

class LogExpression : public OperationExpression {
 public:
  explicit LogExpression(std::vector<Expression *> params) :
      OperationExpression("logb", params) {}

  virtual double evaluate() const {
    double denominator = log(paramValues[1]->evaluate());
    if (isinf(-1 * denominator)) {
      return 0.0 / 0.0;
    }

    double ans = log(paramValues[0]->evaluate()) / denominator;
    return ans;
  }

  virtual ~LogExpression() {}
};

class SelExpression : public OperationExpression {
 public:
  explicit SelExpression(std::vector<Expression *> params) :
      OperationExpression("select", params) {}

  virtual double evaluate() const {
    double ans;
    if (paramValues[0]->evaluate() >= 0) {
      ans = paramValues[1]->evaluate();
    }
    else {
      ans = paramValues[2]->evaluate();
    }
    return ans;
  }

  virtual ~SelExpression() {}
};

class FloorExpression : public OperationExpression {
 public:
  explicit FloorExpression(std::vector<Expression *> params) :
      OperationExpression("floor", params) {}

  virtual double evaluate() const {
    double ans = floor(paramValues[0]->evaluate());
    return ans;
  }

  virtual ~FloorExpression() {}
};

class CeilExpression : public OperationExpression {
 public:
  explicit CeilExpression(std::vector<Expression *> params) :
      OperationExpression("ceil", params) {}

  virtual double evaluate() const {
    double ans = ceil(paramValues[0]->evaluate());
    return ans;
  }

  virtual ~CeilExpression() {}
};

class RoundExpression : public OperationExpression {
 public:
  explicit RoundExpression(std::vector<Expression *> params) :
      OperationExpression("round", params) {}

  virtual double evaluate() const {
    double ans = round(paramValues[0]->evaluate());
    return ans;
  }

  virtual ~RoundExpression() {}
};
