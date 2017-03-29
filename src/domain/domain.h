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
  explicit domain(int dimension, std::unique_ptr<prox> prox)
      : dimension_(dimension), prox_(std::move(prox)) {}
  ~domain() {}

  int dimension() { return dimension_; }
  int diameter() { return diameter_; }
  prox& prox_function() { return *prox_; }

  // support function: argmax_{x\in D} g'x
  // returns <value, argmax>
  virtual std::tuple<double, vector_t> support(
      vector_t const& g) = 0;

 private:
  int dimension_;
  int diameter_;
  std::unique_ptr<prox> prox_;
};

#endif //ROBUST_CPP_DOMAIN_H
