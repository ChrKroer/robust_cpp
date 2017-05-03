//
// Created by Christian Kroer on 4/03/17.
//

#include "./nominal_program_dense.h"
#include "./robust_file_based_program.h"

robust_file_based_program::robust_file_based_program(
    std::string nominal_model_path)
    : nominal_model_path_(nominal_model_path),
      nominal_program_(new nominal_program_dense(nominal_model_path)) {}

void robust_file_based_program::add_uncertainty_constraint(
    int constraint_id, std::unique_ptr<uncertainty_constraint> set) {
  robust_rows_.insert(constraint_id);
  uncertainty_constraints_[constraint_id] = std::move(set);
}
