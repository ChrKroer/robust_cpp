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

  virtual vector_t center() = 0;
  virtual std::tuple<double, vector_t> bregman(
      double alpha, vector_t g, double beta, vector_t y) = 0;
  virtual std::tuple<double, vector_t> mapping(
      double alpha, vector_t g, double beta) = 0;

 private:
  int dimension_;
};

#endif //ROBUST_CPP_PROX_H
