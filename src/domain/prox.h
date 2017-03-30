//
// Created by Christian Kroer on 3/26/17.
//

#ifndef ROBUST_CPP_PROX_H
#define ROBUST_CPP_PROX_H

#include <Eigen/Core>
#include <tuple>
#include "../basic_types.h"

class prox {
 public:
  explicit prox(int dimension) : dimension_(dimension) {}
  ~prox() {}

  int dimension() { return dimension_; }

  virtual vector_d center() = 0;
  virtual std::tuple<double, vector_d> bregman(
      double alpha, vector_d g, double beta, vector_d y) = 0;
  virtual std::tuple<double, vector_d> mapping(
      double alpha, vector_d g, double beta) = 0;

 private:
  int dimension_;
};

#endif //ROBUST_CPP_PROX_H
