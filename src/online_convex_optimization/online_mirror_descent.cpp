//
// Created by Christian Kroer on 3/30/17.
//

#include "./online_mirror_descent.h"

online_mirror_descent::online_mirror_descent(std::unique_ptr<domain> dom)
    : domain_(std::move(dom)) {
  init();
}

void online_mirror_descent::init() {
  iterate_ = get_domain().get_prox().center();
  stepsize_ = 1.0; // TODO: make sure this will work
}

void online_mirror_descent::receive_gradient(vector_d g) {
  std::tuple<double, vector_d> tup =
      get_domain().get_prox().bregman(1.0, g, stepsize_, iterate_);
  iterate_ = std::get<1>(tup);
}
