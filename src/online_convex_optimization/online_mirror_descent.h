//
// Created by Christian Kroer on 3/30/17.
//

#ifndef ROBUST_CPP_ONLINE_MIRROR_DESCENT_H
#define ROBUST_CPP_ONLINE_MIRROR_DESCENT_H

#include <Eigen/Core>
#include <tuple>
#include "../basic_types.h"
#include "./online_gradient_method.h"

class online_mirror_descent : public online_gradient_method {
 public:
  explicit online_mirror_descent(const domain *dom, double stepsize = 1.0);

  void receive_gradient(vector_d g) override;
  const vector_d &get_current_solution() override { return iterate_; }
  const domain &get_domain() override { return *domain_; }

 private:
  void init();

  int num_iterations_ = 0;
  vector_d iterate_;
  const domain *domain_;
  double stepsize_scalar_;

  double stepsize();
};

#endif  // ROBUST_CPP_ONLINE_MIRROR_DESCENT_H
