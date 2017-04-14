//
// Created by Christian Kroer on 4/04/17.
//

#ifndef ROBUST_CPP_EUCLIDEAN_BALL_UNCERTAINTY_SET_H
#define ROBUST_CPP_EUCLIDEAN_BALL_UNCERTAINTY_SET_H

#include "../domain/euclidean_ball.h"
#include "./uncertainty_set.h"

class euclidean_ball_uncertainty_set : public uncertainty_set {
public:
  euclidean_ball_uncertainty_set(int dimension, double radius, vector_d center,
                                 uncertainty_set::function_type func_type)
      : domain_(dimension, radius, center), function_type_(func_type) {}
  ~euclidean_ball_uncertainty_set() {}
  uncertainty_set::uncertainty_set_type get_set_type() const override {
    return uncertainty_set::EUCLIDEAN_BALL;
  }
  uncertainty_set::function_type get_function_type() const override {
    return function_type_;
  }
  int dimension() const override { return domain_.dimension(); }
  vector_d center() const override { return domain_.get_center(); }
  std::pair<double, vector_d> maximizer(vector_d current) const override;

private:
  euclidean_ball domain_;
  uncertainty_set::function_type function_type_;
};

#endif // ROBUST_CPP_EUCLIDEAN_BALL_EUCLIDEAN_BALL_UNCERTAINTY_SET_H
