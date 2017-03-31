//
// Created by Christian Kroer on 3/26/17.
//

#ifndef ROBUST_CPP_SIMPLEX_H
#define ROBUST_CPP_SIMPLEX_H

#include <math.h>
#include "./domain.h"

class simplex : public domain {
 public:
  simplex(int dimension);
  virtual ~simplex();

  int dimension() override { return dimension_; }
  prox& prox_function() override { return *prox_; }
  int diameter() override { return log(dimension()); }
  std::tuple<double, vector_d> support(
      vector_d const& g) override;

 private:
  int dimension_;
  int diameter_;
  std::unique_ptr<prox> prox_;
};

#endif //ROBUST_CPP_SIMPLEX_H
