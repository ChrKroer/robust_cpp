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
  ~simplex();

  int diameter() { return log(dimension()); }
  virtual std::tuple<double, vector_d> support(
      vector_d const& g);
};

#endif //ROBUST_CPP_SIMPLEX_H
