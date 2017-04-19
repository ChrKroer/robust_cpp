//
// Created by Christian Kroer on 3/26/17.
//

#ifndef ROBUST_CPP_UNCERTAINTY_CONSTRAINT_H
#define ROBUST_CPP_UNCERTAINTY_CONSTRAINT_H

#include "./../domain/domain.h"
#include "./../basic_types.h"

class uncertainty_constraint {
public:
  enum constraint_type { EUCLIDEAN_BALL, SIMPLEX, POINT };
  enum function_type { LINEAR };
  ~uncertainty_constraint() {}
  virtual constraint_type get_set_type() const = 0;
  virtual function_type get_function_type() const = 0;
  virtual int dimension() const = 0;
  virtual vector_d center() const = 0;
  virtual double radius() const = 0;
  // returns the parameter instantiation that maximizes the constraint function,
  // as well as the constraint value.
  virtual std::pair<double, vector_d> maximizer(const vector_d current) const = 0;
  virtual vector_d gradient(const vector_d &current) const = 0;
  virtual const domain &get_domain() const = 0;
};

#endif // ROBUST_CPP_UNCERTAINTY_CONSTRAINT_H
