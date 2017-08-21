//
// Created by Christian Kroer on 3/26/17.
//

#ifndef ROBUST_CPP_PROX_H
#define ROBUST_CPP_PROX_H

#include <Eigen/Core>
#include <tuple>
#include "../basic_types.h"

/**
 * Class for representing proximal problems of the form
 * arg min_{x \in X} \alpha g'x + \beta D(x, y)
 * where D is a Bregman divergence
 */
class prox {
 public:
  virtual ~prox() = default;
  virtual vector_d center() const = 0;
  virtual std::tuple<double, vector_d> bregman(double alpha, const vector_d &g,
                                               double beta,
                                               const vector_d &y) const = 0;
  virtual std::tuple<double, vector_d> mapping(double alpha, const vector_d &g,
                                               double beta) const = 0;
};

#endif  // ROBUST_CPP_PROX_H
