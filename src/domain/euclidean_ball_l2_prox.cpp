#include "./euclidean_ball_l2_prox.h"
#include <iostream>

euclidean_ball_l2_prox::euclidean_ball_l2_prox(int dimension, double radius,
                                               vector_d center)
    : dimension_(dimension), radius_(radius), center_(center) {}

vector_d euclidean_ball_l2_prox::center() const {
  vector_d zeroes = vector_d::Zero(dimension());
  std::tuple<double, vector_d> t = mapping(0.0, zeroes, 1.0);
  return std::get<1>(t);
}

std::tuple<double, vector_d>
euclidean_ball_l2_prox::bregman(double alpha, vector_d &g, double beta,
                                vector_d &y) const {
  vector_d unconstrained_sol = y - (alpha / beta) * g;
  vector_d proj = radius_ * (unconstrained_sol - center_) /
                      (unconstrained_sol - center_).norm() +
                  center_;
  double val = alpha * g.dot(proj) + (beta / 2) * (proj).squaredNorm();
  return std::make_pair(val, proj);
}

std::tuple<double, vector_d>
euclidean_ball_l2_prox::mapping(double alpha, vector_d &g, double beta) const {
  vector_d zeroes = vector_d::Zero(dimension());
  return bregman(alpha, g, beta, zeroes);
}
