//
// Created by Christian Kroer on 3/29/17.
//

#ifndef ROBUST_CPP_ONLINE_GRADIENT_METHOD_H
#define ROBUST_CPP_ONLINE_GRADIENT_METHOD_H

#include "../basic_types.h"
#include "../domain/domain.h"
#include <Eigen/Core>
#include <tuple>

class online_gradient_method {
public:
  virtual ~online_gradient_method() {}
  virtual void receive_gradient(vector_d g) = 0;
  virtual const vector_d &get_current_solution() = 0;
  virtual const domain &get_domain() = 0;
};

#endif // ROBUST_CPP_ONLINE_GRADIENT_METHOD_H
