//
// Created by Christian Kroer on 3/26/17.
//

#ifndef ROBUST_CPP_DOMAIN_H
#define ROBUST_CPP_DOMAIN_H

#include <tuple>
#include <Eigen/Core>
#include "../basic_types.h"
#include "./prox.h"

class domain {
 public:
  virtual ~domain() {}
  virtual int dimension() = 0;
  virtual int diameter() = 0;
  virtual prox& prox_function() = 0;
  // support function: argmax_{x\in D} g'x
  // returns <value, argmax>
  virtual std::tuple<double, vector_d> support(
      vector_d const& g) = 0;
};

#endif //ROBUST_CPP_DOMAIN_H
