//
// Created by Christian Kroer on 3/30/17.
//

#ifndef ROBUST_CPP_ONLINE_DUAL_AVERAGING_H
#define ROBUST_CPP_ONLINE_DUAL_AVERAGING_H

#include <Eigen/Core>
#include <tuple>
#include "../basic_types.h"
#include "./online_gradient_method.h"

class online_dual_averaging : public online_gradient_method {
 public:
  explicit online_dual_averaging(const domain *dom,
                                 double stepsize_scalar = 1.0);

  void receive_gradient(vector_d g) override;
  const vector_d &get_current_solution() override { return iterate_; }
  const domain &get_domain() override { return *domain_; }

 private:
  void init();

  int num_iterations_ = 0;
  vector_d iterate_;
  vector_d gradient_average_;
  const domain *domain_;
  double stepsize_scalar_;

  double prox_weight();
};

#endif  // ROBUST_CPP_ONLINE_DUAL_AVERAGING_H
