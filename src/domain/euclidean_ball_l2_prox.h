#ifndef ROBUST_CPP_EUCLIDEAN_BALL_L2_PROX_H
#define ROBUST_CPP_EUCLIDEAN_BALL_L2_PROX_H

#include "../basic_types.h"
#include "./prox.h"
#include <Eigen/Core>

/////////////////////////////////////////////////////////
// represents the proximal operator                    //
// min_{x in ||x-c||<= r} alpha*g'x + beta/2*||x-y||^2 //
/////////////////////////////////////////////////////////
class euclidean_ball_l2_prox : public prox {
public:
  euclidean_ball_l2_prox(int dimension, double radius);
  euclidean_ball_l2_prox(int dimension, double radius, vector_d center);

  int dimension() const { return dimension_; }
  vector_d center() const override;
  std::tuple<double, vector_d> bregman(double alpha, const vector_d &g, double beta,
                                       const vector_d &y) const override;
  std::tuple<double, vector_d> mapping(double alpha, const vector_d &g,
                                       double beta) const override;

private:
  int dimension_;
  double radius_;
  vector_d center_;
};

#endif // ROBUST_CPP_EUCLIDEAN_BALL_L2_PROX_H
