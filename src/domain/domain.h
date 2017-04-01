//
// Created by Christian Kroer on 3/26/17.
//

#ifndef ROBUST_CPP_DOMAIN_H
#define ROBUST_CPP_DOMAIN_H

#include "../basic_types.h"
#include "./prox.h"
#include <Eigen/Core>
#include <tuple>

class domain {
public:
  virtual ~domain() {}
  virtual int dimension() const = 0;
  virtual int diameter() const = 0;
  virtual const prox &get_prox() const = 0;
  // support function: argmax_{x\in D} g'x
  // returns <value, argmax>
  virtual std::tuple<double, vector_d> support(vector_d const &g) const = 0;
};

#endif // ROBUST_CPP_DOMAIN_H
