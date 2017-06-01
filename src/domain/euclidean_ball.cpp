//
// Created by Christian Kroer on 4/01/17.
//

#include "euclidean_ball.h"
#include "euclidean_ball_l2_prox.h"

euclidean_ball::euclidean_ball(int dimension, double radius)
    : dimension_(dimension), radius_(radius),
      prox_(std::unique_ptr<euclidean_ball_l2_prox>(
          new euclidean_ball_l2_prox(dimension, radius))) {}

euclidean_ball::euclidean_ball(int dimension, double radius, vector_d center)
    : dimension_(dimension), radius_(radius), center_(center),
      prox_(std::unique_ptr<euclidean_ball_l2_prox>(
          new euclidean_ball_l2_prox(dimension, radius, center))) {}

std::pair<double, vector_d> euclidean_ball::support(vector_d const &g) const {
  double normalizer = g.norm() / radius_;
  vector_d argmax = g;
  if (g.norm() > radius_) {
    argmax /= normalizer;
  }
  if (center_.size() > 0) {
    return std::make_pair(g.dot(argmax + center_), argmax + center_);
  } else {
    return std::make_pair(g.dot(argmax), argmax);
  }
}
