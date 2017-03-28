
#ifndef ROBUST_CPP_SIMPLEX_ENTROPY_PROX_H
#define ROBUST_CPP_SIMPLEX_ENTROPY_PROX_H

#include <Eigen/Core>
#include "./prox.h"
#include "../basic_types.h"

class SimplexEntropyProx : public Prox {
 public:
  SimplexEntropyProx(int dimension);
  ~SimplexEntropyProx();

  virtual Eigen::VectorXd center();
  virtual std::tuple<double, vector_t> bregman(
      double alpha, Eigen::VectorXd g, double beta, Eigen::VectorXd y);
  virtual std::tuple<double, vector_t> mapping(
      double alpha, Eigen::VectorXd g, double beta);
};

#endif //ROBUST_CPP_SIMPLEX_ENTROPY_PROX_H
