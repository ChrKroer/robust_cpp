//
// Created by Christian Kroer on 3/26/17.
//

#ifndef ROBUST_CPP_SIMPLEX_H
#define ROBUST_CPP_SIMPLEX_H

#include "./domain.h"
#include <memory>
#include <cmath>

class simplex : public domain {
public:
  simplex(int dimension);
  int dimension() const override { return dimension_; }
  const prox &get_prox() const override { return *prox_; }
  int diameter() const override { return std::log(dimension()); }
  std::pair<double, vector_d> support(vector_d const &g) const override;

private:
  int dimension_;
  int diameter_;
  std::unique_ptr<prox> prox_;
};

#endif // ROBUST_CPP_SIMPLEX_H
