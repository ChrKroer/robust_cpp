//
// Created by Christian Kroer on 3/30/17.
//

#ifndef ROBUST_CPP_ONLINE_ADAM_H
#define ROBUST_CPP_ONLINE_ADAM_H

#include <Eigen/Core>
#include <tuple>
#include "../basic_types.h"
#include "./online_gradient_method.h"

class online_adam : public online_gradient_method {
 public:
  explicit online_adam(const domain *dom, double alpha = 1.0,
                       double beta1 = 0.9, double beta2 = 0.999,
                       double epsilon = 1e-8);

  void receive_gradient(vector_d g) override;
  const vector_d &get_current_solution() override { return iterate_; }
  void set_solution(vector_d solution) override { iterate_ = solution; }
  const domain &get_domain() override { return *domain_; }

 private:
  void init();

  int num_iterations_ = 0;
  vector_d iterate_;
  vector_d first_moment_, second_moment_;
  const domain *domain_;
  double alpha_, beta1_, beta2_, epsilon_;
};

#endif  // ROBUST_CPP_ONLINE_ADAM_H
