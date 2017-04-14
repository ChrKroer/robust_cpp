//
// Created by Christian Kroer on 3/26/17.
//

#ifndef ROBUST_CPP_UNCERTAINTY_SET_H
#define ROBUST_CPP_UNCERTAINTY_SET_H

#include "./../domain/domain.h"

class uncertainty_set {
public:
  enum uncertainty_set_type { EUCLIDEAN_BALL, SIMPLEX, POINT };
  enum function_type { LINEAR };
  ~uncertainty_set() {}
  virtual uncertainty_set_type get_set_type() const = 0;
  virtual function_type get_function_type() const = 0;
  virtual int dimension() const = 0;
  virtual vector_d center() const = 0;
  // returns the parameter instantiation that maximizes the constraint function,
  // as well as the constraint value.
  virtual std::pair<double, vector_d> maximizer(vector_d current) const = 0;
};

#endif // ROBUST_CPP_UNCERTAINTY_SET_H
