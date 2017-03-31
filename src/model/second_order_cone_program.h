//
// Created by Christian Kroer on 3/26/17.
//

#ifndef ROBUST_CPP_SECOND_ORDER_CONE_PROGRAM_H
#define ROBUST_CPP_SECOND_ORDER_CONE_PROGRAM_H

class second_order_cone_program {
 public:
  explicit second_order_cone_program(int dimension) : dimension_(dimension) {}
  ~second_order_cone_program() {}

  int dimension() { return dimension_; }
  int num_constraints() { return num_constraints_; }

 private:
  int dimension_;
  int num_constraints_;
};

#endif //ROBUST_CPP_SECOND_ORDER_CONE_PROGRAM_H
