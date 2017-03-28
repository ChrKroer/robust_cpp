//
// Created by Christian Kroer on 3/26/17.
//

#ifndef ROBUST_CPP_DOMAIN_H
#define ROBUST_CPP_DOMAIN_H

#include <tuple>
#include <Eigen/Core>
#include "../basic_types.h"
#include "./prox.h"

class Domain {
 public:
  explicit Domain(int dimension, std::unique_ptr<Prox> prox)
      : dimension_(dimension), prox_(std::move(prox)) {}
  ~Domain() {}

  int dimension() { return dimension_; }
  int diameter() { return diameter_; }
  Prox& prox() { return *prox_; }

  // support function: argmax_{x\in D} g'x
  // returns <value, argmax>
  virtual std::tuple<double, vector_t> support(
      vector_t const& g) = 0;

 private:
  int dimension_;
  int diameter_;
  std::unique_ptr<Prox> prox_;
};

#endif //ROBUST_CPP_DOMAIN_H
