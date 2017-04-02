//
// Created by Christian Kroer on 4/01/17.
//

#ifndef ROBUST_CPP_EUCLIDEAN_BALL_H
#define ROBUST_CPP_EUCLIDEAN_BALL_H

#include "./domain.h"
#include <math.h>

////////////////////////////////////////////////////////////////////
// represents a Euclidean ball {x : ||x - c|| <= r} centered at c //
////////////////////////////////////////////////////////////////////
class euclidean_ball : public domain {
public:
  euclidean_ball(int dimension, double radius, double center);
  int dimension() const override { return dimension_; }
  const prox &get_prox() const override { return *prox_; }
  int diameter() const override { return log(dimension()); }
  std::tuple<double, vector_d> support(vector_d const &g) const override;

private:
  double radius_, center_;
  int dimension_;
  int diameter_;
  std::unique_ptr<prox> prox_;
};

#endif // ROBUST_CPP_EUCLIDEAN_BALL_H
