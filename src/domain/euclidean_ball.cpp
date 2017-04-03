//
// Created by Christian Kroer on 4/01/17.
//

#include "euclidean_ball.h"
#include "euclidean_ball_l2_prox.h"

euclidean_ball::euclidean_ball(int dimension, double radius, vector_d center)
    : dimension_(dimension), radius_(radius), center_(center),
      prox_(std::unique_ptr<euclidean_ball_l2_prox>(
          new euclidean_ball_l2_prox(dimension, radius, center))) {}

std::tuple<double, vector_d> euclidean_ball::support(vector_d const &g) const {
  double normalizer = g.norm() * radius_;
  vector_d argmax = g / normalizer;
  return std::make_tuple(normalizer + g.dot(center_), argmax + center_);
}
