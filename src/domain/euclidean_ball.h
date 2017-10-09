//
// Created by Christian Kroer on 4/01/17.
//

#ifndef ROBUST_CPP_EUCLIDEAN_BALL_H
#define ROBUST_CPP_EUCLIDEAN_BALL_H

#include <cmath>
#include <memory>
#include "./domain.h"

////////////////////////////////////////////////////////////////////
// represents a Euclidean ball {x : ||x - c|| <= r} centered at c //
////////////////////////////////////////////////////////////////////
class euclidean_ball : public domain {
 public:
  euclidean_ball(int dimension, double radius);
  euclidean_ball(int dimension, double radius, vector_d center);
  int dimension() const override { return dimension_; }
  const prox &get_prox() const override { return *prox_; }
  void project(vector_d *x) const override { *x /= (*x).norm(); }
  int diameter() const override { return std::log(dimension()); }
  std::pair<double, vector_d> support(vector_d const &g) const override;
  const vector_d &get_center() const { return center_; }
  double get_radius() const { return radius_; }

 private:
  int dimension_;
  double radius_;
  vector_d center_;
  std::unique_ptr<prox> prox_;
};

#endif  // ROBUST_CPP_EUCLIDEAN_BALL_H
