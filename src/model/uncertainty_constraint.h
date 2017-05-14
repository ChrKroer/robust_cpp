//
// Created by Christian Kroer on 3/26/17.
//

#ifndef ROBUST_CPP_UNCERTAINTY_CONSTRAINT_H
#define ROBUST_CPP_UNCERTAINTY_CONSTRAINT_H

#include "./../basic_types.h"
#include "./../domain/domain.h"

class uncertainty_constraint {
public:
  // enum constraint_type { EUCLIDEAN_BALL, SIMPLEX, POINT };
  enum function_type { LINEAR, QUADRATIC };
  ~uncertainty_constraint() {}
  virtual function_type get_function_type() const = 0;
  virtual int dimension() const = 0;
  virtual int get_constraint_id() const = 0;
  // returns the parameter instantiation that maximizes the constraint function,
  // as well as the constraint value.
  virtual std::pair<double, vector_d>
  maximizer(const vector_d current) const = 0;
  virtual vector_d gradient(const vector_d &current) const = 0;
  virtual const domain *get_domain() const = 0;
  virtual double violation_amount(const vector_d &solution,
                                  const vector_d &constraint_params) const = 0;
};

#endif // ROBUST_CPP_UNCERTAINTY_CONSTRAINT_H
