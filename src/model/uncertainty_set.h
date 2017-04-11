//
// Created by Christian Kroer on 3/26/17.
//

#ifndef ROBUST_CPP_UNCERTAINTY_SET_H
#define ROBUST_CPP_UNCERTAINTY_SET_H

#include "./../domain/domain.h"

class uncertainty_set {
public:
  enum uncertainty_type { euclidean_ball, simplex, point };
  ~uncertainty_set() {}
  virtual uncertainty_type type() const = 0;
  virtual int dimension() const = 0;
  virtual vector_d center() const = 0;
  virtual std::pair<double, vector_d> maximizer(vector_d current) const = 0;
};

#endif // ROBUST_CPP_UNCERTAINTY_SET_H
