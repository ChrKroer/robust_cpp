//
// Created by Christian Kroer on 3/30/17.
//

#ifndef ROBUST_CPP_ONLINE_MIRROR_DESCENT_H
#define ROBUST_CPP_ONLINE_MIRROR_DESCENT_H

#include "../basic_types.h"
#include "../domain/domain.h"
#include "./online_gradient_method.h"
#include <Eigen/Core>
#include <tuple>

class online_mirror_descent : online_gradient_method {
public:
  online_mirror_descent(std::unique_ptr<domain> domain);
  virtual ~online_mirror_descent() {}

  void receive_gradient(vector_d g) override;
  const vector_d &get_current_solution() override { return iterate_; }
  const domain &get_domain() override { return *domain_; }

private:
  void init();

  double stepsize_;
  vector_d iterate_;
  std::unique_ptr<domain> domain_;
};

#endif // ROBUST_CPP_ONLINE_MIRROR_DESCENT_H
