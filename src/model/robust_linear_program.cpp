//
// Created by Christian Kroer on 4/03/17.
//

#include "./robust_linear_program.h"
#include "./nominal_program_dense.h"

robust_linear_program::robust_linear_program(std::string nominal_model_path)
    : nominal_model_path_(nominal_model_path),
      nominal_program_(new nominal_program_dense(nominal_model_path)) {}

void robust_linear_program::add_uncertainty_constraint(
    int constraint_id, std::unique_ptr<uncertainty_constraint> set) {
  robust_rows_.insert(constraint_id);
  uncertainty_constraints_[constraint_id] = std::move(set);
}
