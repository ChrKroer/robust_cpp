//
// Created by Christian Kroer on 4/12/17.
//

#include "./euclidean_ball_uncertainty_constraint.h"
#include "./../logging.h"

std::pair<double, vector_d>
euclidean_ball_uncertainty_constraint::maximizer(const vector_d current) const {
  vector_d g = gradient(current);
  return domain_.support(g);
};

vector_d
euclidean_ball_uncertainty_constraint::gradient(const vector_d &current) const {
  if (function_type_ == uncertainty_constraint::LINEAR) {
    vector_d g = vector_d(domain_.dimension());
    for (int i = 0; i < domain_.dimension(); i++) {
      g(i) = current(uncertainty_variable_ids_[i]);
    }
    return g;
  } else {
    logger->error("Unsupported function type");
    exit(1);
  }
}
