#ifndef ROBUST_CPP_SIMPLEX_ENTROPY_PROX_H
#define ROBUST_CPP_SIMPLEX_ENTROPY_PROX_H

#include "../basic_types.h"
#include "./prox.h"
#include <Eigen/Core>

class simplex_entropy_prox : public prox {
public:
  simplex_entropy_prox(int dimension);

  int dimension() const { return dimension_; }
  vector_d center() const override;
  std::tuple<double, vector_d> bregman(double alpha, const vector_d &g, double beta,
                                       const vector_d &y) const override;
  std::tuple<double, vector_d> mapping(double alpha, const vector_d &g,
                                       double beta) const override;

private:
  int dimension_;
};

#endif // ROBUST_CPP_SIMPLEX_ENTROPY_PROX_H
