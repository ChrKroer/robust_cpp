//
// Created by Christian Kroer on 3/30/17.
//

#include <math.h>
#include "./online_mirror_descent.h"

online_mirror_descent::online_mirror_descent(const domain *dom, double stepsize)
    : domain_(dom), stepsize_scalar_(stepsize) {
  init();
}

void online_mirror_descent::init() {
  iterate_ = get_domain().get_prox().center();
}

void online_mirror_descent::receive_gradient(vector_d g) {
  std::tuple<double, vector_d> tup =
      get_domain().get_prox().bregman(stepsize(), g, 1.0, iterate_);
  iterate_ = std::get<1>(tup);
  num_iterations_++;
}

double online_mirror_descent::stepsize() {
  // R/L * sqrt(2rho/t)
  // TODO: set this more rigorously;
  return stepsize_scalar_ / sqrt(num_iterations_ + 1);
}
