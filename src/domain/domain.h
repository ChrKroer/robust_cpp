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
  explicit Domain(int dimension) : _dimension(dimension) {}
  ~Domain() {}

  int dimension() { return _dimension; }
  int diameter() { return _diameter; }
  Prox& prox() { return *_prox; }

  // support function: argmax_{x\in D} g'x
  // returns <value, argmax>
  virtual std::tuple<double, vector_t> support(
      vector_t const& g) = 0;

 private:
  int _dimension;
  int _diameter;
  std::unique_ptr<Prox> _prox;
};

#endif //ROBUST_CPP_DOMAIN_H
