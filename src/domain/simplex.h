//
// Created by Christian Kroer on 3/26/17.
//

#ifndef ROBUST_CPP_SIMPLEX_H
#define ROBUST_CPP_SIMPLEX_H

#include "./domain.h"

class Simplex : public Domain {
 public:
  Simplex(int dimension);
  ~Simplex();

  virtual std::tuple<double, vector_t> support(
      vector_t const& g);
};

#endif //ROBUST_CPP_SIMPLEX_H
