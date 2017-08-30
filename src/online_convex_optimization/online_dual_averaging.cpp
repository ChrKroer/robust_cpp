//
// Created by Christian Kroer on 3/30/17.
//

#include <math.h>
#include "./online_dual_averaging.h"

online_dual_averaging::online_dual_averaging(const domain *dom,
                                             double stepsize_scalar)
    : domain_(dom), stepsize_scalar_(stepsize_scalar) {
  init();
}

void online_dual_averaging::init() {
  gradient_average_ = vector_d::Zero(get_domain().dimension());
  iterate_ = get_domain().get_prox().center();
}

void online_dual_averaging::receive_gradient(vector_d g) {
  num_iterations_++;
  gradient_average_ =
      ((num_iterations_ - 1.0) / num_iterations_) * gradient_average_ +
      (1.0 / num_iterations_) * g;
  std::tuple<double, vector_d> tup =
      get_domain().get_prox().mapping(1.0, gradient_average_, prox_weight());
  iterate_ = std::get<1>(tup);
}

double online_dual_averaging::prox_weight() {
  // R/L * sqrt(2rho/t)
  // TODO: set this more rigorously;
  double beta_t = sqrt(num_iterations_) / stepsize_scalar_;
  return beta_t / num_iterations_;
}
