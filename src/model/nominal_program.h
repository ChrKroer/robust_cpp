//
// Created by Christian Kroer on 3/26/17.
//

#ifndef ROBUST_CPP_NOMINAL_PROGRAM_H
#define ROBUST_CPP_NOMINAL_PROGRAM_H

#include "ClpModel.hpp"

class nominal_program {
public:
  explicit nominal_program(std::string model_path);
  ~nominal_program() {}

  int dimension() { return clp_model_.getNumCols(); }
  int num_constraints() { return clp_model_.getNumRows(); }

private:
  std::string model_path_;
  ClpModel clp_model_;
};

#endif // ROBUST_CPP_NOMINAL_PROGRAM_H
