//
// Created by Christian Kroer on 4/01/17.
//

#include "euclidean_ball.h"

euclidean_ball::euclidean_ball(int dimension, double radius, vector_d center)
    : dimension_(dimension), radius_(radius), center_(center) {}

std::tuple<double, vector_d> euclidean_ball::support(vector_d const &g) const {
  double length = g.norm();
  vector_d argmax = g / length;
  return std::make_tuple(length, argmax);
}
