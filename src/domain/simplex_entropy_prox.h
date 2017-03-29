
#ifndef ROBUST_CPP_SIMPLEX_ENTROPY_PROX_H
#define ROBUST_CPP_SIMPLEX_ENTROPY_PROX_H

#include <Eigen/Core>
#include "./prox.h"
#include "../basic_types.h"

class simplex_entropy_prox : public prox {
 public:
  simplex_entropy_prox(int dimension);
  ~simplex_entropy_prox();

  virtual vector_t center();
  virtual std::tuple<double, vector_t> bregman(
      double alpha, vector_t g, double beta, vector_t y);
  virtual std::tuple<double, vector_t> mapping(
      double alpha, vector_t g, double beta);
};

#endif //ROBUST_CPP_SIMPLEX_ENTROPY_PROX_H
