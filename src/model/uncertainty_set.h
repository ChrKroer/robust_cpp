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
  virtual uncertainty_type type() = 0;
  virtual int dimension() = 0;
  virtual vector_d center() = 0;
  virtual std::pair<double, vector_d> maximizer(vector_d current) = 0;
  virtual std::pair<double, vector_d> oco_update(vector_d current) = 0;
};

#endif // ROBUST_CPP_UNCERTAINTY_SET_H
