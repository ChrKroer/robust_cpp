//
// Created by Christian Kroer on 3/30/17.
//

#include <math.h>
#include "./online_adam.h"

online_adam::online_adam(const domain *dom, double alpha, double beta1,
                         double beta2, double epsilon)
    : domain_(dom),
      alpha_(alpha),
      beta1_(beta1),
      beta2_(beta2),
      epsilon_(epsilon) {
  init();
}

void online_adam::init() {
  first_moment_ = vector_d::Zero(get_domain().dimension());
  second_moment_ = vector_d::Zero(get_domain().dimension());
  iterate_ = get_domain().get_prox().center();
}

void online_adam::receive_gradient(vector_d g) {
  num_iterations_++;
  first_moment_ = beta1_ * first_moment_ + (1 - beta1_) * g;
  second_moment_ = beta2_ * second_moment_ + (1 - beta2_) * g.cwiseProduct(g);
  double alpha_t = alpha_ * sqrt(1 - pow(beta2_, num_iterations_)) /
                   (1 - pow(beta1_, num_iterations_));
  iterate_ = iterate_.array() -
             alpha_t * first_moment_.array() /
                 (second_moment_.array().sqrt() + epsilon_);
  domain_->project(&iterate_);
}
