//
// Created by Christian Kroer on 4/12/17.
//

#include "./euclidean_ball_uncertainty_set.h"
#include "./../logging.h"

std::pair<double, vector_d>
euclidean_ball_uncertainty_set::maximizer(vector_d current) const {
  if (get_function_type() == uncertainty_set::function_type::LINEAR) {
    return domain_.support(current);
  } else {
    logger->error("Unsupported function type");
    exit(1);
  }
};
