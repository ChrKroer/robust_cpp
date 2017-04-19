//
// Created by Christian Kroer on 4/12/17.
//

#include "./euclidean_ball_uncertainty_constraint.h"
#include "./../logging.h"

std::pair<double, vector_d>
euclidean_ball_uncertainty_constraint::maximizer(vector_d current) const {
  if (get_function_type() == uncertainty_constraint::function_type::LINEAR) {
    return domain_.support(current);
  } else {
    logger->error("Unsupported function type");
    exit(1);
  }
};

vector_d
euclidean_ball_uncertainty_constraint::gradient(vector_d &current) const {
  if (function_type_ == uncertainty_constraint::LINEAR) {
    return current;
  } else {
    logger->error("Unsupported function type");
    exit(1);
  }
}
