//
// Created by Christian Kroer on 3/30/17.
//

#include "./online_mirror_descent.h"

online_mirror_descent::online_mirror_descent(std::unique_ptr<domain>) :
    domain_(std::move(domain))
     { }

void online_mirror_descent::init() {
  iterate_ = domain()->prox_function().center();
}

void online_mirror_descent::receive_gradient(vector_d g) {
  std::tuple<double, vector_d> tup = domain().prox().bregman(
      1.0, g, stepsize, iterate_);
  iterate_ = std::get<1>(tup);
}
