//
// Created by Christian Kroer on 3/30/17.
//

#ifndef ROBUST_CPP_ONLINE_MIRROR_DESCENT_H
#define ROBUST_CPP_ONLINE_MIRROR_DESCENT_H

#include <Eigen/Core>
#include <tuple>
#include "../basic_types.h"

class online_mirror_descent : online_gradient_method {
 public:
  explicit online_mirror_descent(std::unique_ptr<domain> dom) :
      domain_(std::move(dom))
  {}
  virtual ~online_mirror_descent() {}

  virtual void receive_gradient(vector_d g) override;
  virtual const vector_d& iterate() override  { return iterate_; }
  virtual const domain& domain() override { return domain_; }

 private:
  void init();

  vector_d iterate_;
  std::unique_ptr<domain> domain_;
};

#endif //ROBUST_CPP_ONLINE_MIRROR_DESCENT_H
