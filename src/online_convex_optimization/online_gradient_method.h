//
// Created by Christian Kroer on 3/29/17.
//

#ifndef ROBUST_CPP_ONLINE_GRADIENT_METHOD_H
#define ROBUST_CPP_ONLINE_GRADIENT_METHOD_H

#include <Eigen/Core>
#include <tuple>
#include "../basic_types.h"

class online_gradient_method {
 public:
  virtual ~online_gradient_method() {}
  virtual void receive_gradient(vector_d g) = 0;
  virtual const vector_d& iterate() = 0;
  virtual const domain& domain() = 0;
};

#endif //ROBUST_CPP_ONLINE_GRADIENT_METHOD_H

