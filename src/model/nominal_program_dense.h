//
// Created by Christian Kroer on 4/08/17.
//

#ifndef ROBUST_CPP_NOMINAL_PROGRAM_DENSE_H
#define ROBUST_CPP_NOMINAL_PROGRAM_DENSE_H

#include "ClpModel.hpp"
#include "./nominal_program.h"

class nominal_program_dense : nominal_program{
public:
  explicit nominal_program_dense(std::string model_path);
  ~nominal_program_dense() {}

  int dimension() { return clp_model_.getNumCols(); }
  int num_constraints() { return clp_model_.getNumRows(); }

private:
  std::string model_path_;
  ClpModel clp_model_;
};

#endif // ROBUST_CPP_NOMINAL_PROGRAM_DENSE_H
