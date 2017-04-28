//
// Created by Christian Kroer on 4/04/17.
//

#ifndef ROBUST_CPP_EUCLIDEAN_BALL_UNCERTAINTY_CONSTRAINT_H
#define ROBUST_CPP_EUCLIDEAN_BALL_UNCERTAINTY_CONSTRAINT_H

#include "../domain/euclidean_ball.h"
#include "./uncertainty_constraint.h"
#include <vector>

class euclidean_ball_uncertainty_constraint : public uncertainty_constraint {
public:
  euclidean_ball_uncertainty_constraint(
      int dimension, double radius, vector_d center,
      uncertainty_constraint::function_type func_type,
      std::vector<int> uncertainty_variable_ids)
      : domain_(dimension, radius, center), function_type_(func_type),
        uncertainty_variable_ids_(uncertainty_variable_ids) {}
  ~euclidean_ball_uncertainty_constraint() {}
  uncertainty_constraint::constraint_type get_set_type() const override {
    return uncertainty_constraint::EUCLIDEAN_BALL;
  }
  uncertainty_constraint::function_type get_function_type() const override {
    return function_type_;
  }
  int dimension() const override { return domain_.dimension(); }
  vector_d center() const override { return domain_.get_center(); }
  double radius() const override { return domain_.get_radius(); }
  std::pair<double, vector_d> maximizer(const vector_d current) const override;
  vector_d gradient(const vector_d &current) const override;
  const std::vector<int> &uncertainty_variable_ids() const override {
    return uncertainty_variable_ids_;
  }
  const domain &get_domain() const override { return domain_; };

private:
  euclidean_ball domain_;
  uncertainty_constraint::function_type function_type_;
  std::vector<int> uncertainty_variable_ids_;
};

#endif // ROBUST_CPP_EUCLIDEAN_BALL_EUCLIDEAN_BALL_UNCERTAINTY_CONSTRAINT_H
