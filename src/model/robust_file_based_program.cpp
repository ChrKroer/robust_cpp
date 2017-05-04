//
// Created by Christian Kroer on 4/03/17.
//

#include "./robust_file_based_program.h"
#include "./nominal_program_dense.h"

robust_file_based_program::robust_file_based_program(
    std::string nominal_model_path)
    : nominal_model_path_(nominal_model_path),
      nominal_program_(new nominal_program_dense(nominal_model_path)) {}

void robust_file_based_program::add_uncertainty_constraint(
    std::unique_ptr<uncertainty_constraint> constraint) {
  int id = constraint->get_constraint_id();
  robust_rows_.insert(id);
  uncertainty_constraints_[id] = std::move(constraint);
}
