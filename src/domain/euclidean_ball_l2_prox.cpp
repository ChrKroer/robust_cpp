#include "./euclidean_ball_l2_prox.h"
#include "../logging.h"
#include <iostream>

euclidean_ball_l2_prox::euclidean_ball_l2_prox(int dimension, double radius)
    : dimension_(dimension), radius_(radius) {}

euclidean_ball_l2_prox::euclidean_ball_l2_prox(int dimension, double radius,
                                               vector_d center)
    : dimension_(dimension), radius_(radius), center_(center) {}

vector_d euclidean_ball_l2_prox::center() const {
  vector_d zeroes = vector_d::Zero(dimension());
  std::tuple<double, vector_d> t = mapping(1.0, zeroes, 1.0);
  return std::get<1>(t);
}

std::tuple<double, vector_d>
euclidean_ball_l2_prox::bregman(double alpha, const vector_d &g, double beta,
                                const vector_d &y) const {
  vector_d unconstrained_sol = y - (alpha / beta) * g;
  double norm;
  if (center_.size() > 0) {
    norm = (unconstrained_sol - center_).norm();
  } else {
    norm = (unconstrained_sol).norm();
  }
  vector_d proj;
  if (norm < radius_) { // already feasible
    proj = unconstrained_sol;
  } else { // project onto ball
    if (center_.size() > 0) {
      proj = radius_ / norm * (unconstrained_sol - center_) + center_;
    } else {
      proj = radius_ / norm * unconstrained_sol;
    }
  }
  double val = alpha * g.dot(proj) + (beta / 2) * (proj).squaredNorm();
  return std::make_pair(val, proj);
}

std::tuple<double, vector_d>
euclidean_ball_l2_prox::mapping(double alpha, const vector_d &g,
                                double beta) const {
  return bregman(alpha, g, beta, center_);
}
