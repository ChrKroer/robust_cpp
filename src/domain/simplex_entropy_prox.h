
#ifndef ROBUST_CPP_SIMPLEX_ENTROPY_PROX_H
#define ROBUST_CPP_SIMPLEX_ENTROPY_PROX_H

#include <Eigen/Core>
#include "./prox.h"
#include "../basic_types.h"

class simplex_entropy_prox : public prox {
 public:
  simplex_entropy_prox(int dimension);
  ~simplex_entropy_prox();

  virtual vector_d center();
  virtual std::tuple<double, vector_d> bregman(
      double alpha, vector_d g, double beta, vector_d y);
  virtual std::tuple<double, vector_d> mapping(
      double alpha, vector_d g, double beta);
};

#endif //ROBUST_CPP_SIMPLEX_ENTROPY_PROX_H
