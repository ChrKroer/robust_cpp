//
// Created by Christian Kroer on 4/08/17.
//

#ifndef ROBUST_CPP_NOMINAL_PROGRAM_DENSE_H
#define ROBUST_CPP_NOMINAL_PROGRAM_DENSE_H

#include "./nominal_program.h"
#include <string>

class nominal_program_dense : public nominal_program {
public:
  explicit nominal_program_dense(std::string model_path);
  ~nominal_program_dense() {}

  int dimension() { return dimension_; }
  int num_constraints() { return num_constraints_; }

private:
  int dimension_;
  int num_constraints_;
  std::string model_path_;
};

#endif // ROBUST_CPP_NOMINAL_PROGRAM_DENSE_H
