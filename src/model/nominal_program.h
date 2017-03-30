//
// Created by Christian Kroer on 3/26/17.
//

#ifndef ROBUST_CPP_NOMINAL_PROGRAM_H
#define ROBUST_CPP_NOMINAL_PROGRAM_H

class nominal_program {
 public:
  explicit nominal_program(int dimension) : dimension_(dimension) {}
  ~nominal_program() {}

  int dimension() { return dimension_; }
  int num_constraints() { return num_constraints_; }

 private:
  int dimension_;
  int num_constraints_;
};

#endif //ROBUST_CPP_NOMINAL_PROGRAM_H
