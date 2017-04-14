//
// Created by Christian Kroer on 4/03/17.
//

#include "./robust_linear_program.h"
#include "./nominal_program_dense.h"

robust_linear_program::robust_linear_program(std::string nominal_model_path)
    : nominal_model_path_(nominal_model_path),
      nominal_program_(new nominal_program_dense(nominal_model_path)) {}

robust_linear_program::robust_linear_program(
    std::string nominal_model_path, uncertainty_set::uncertainty_set_type t)
    : nominal_model_path_(nominal_model_path),
      nominal_program_(new nominal_program_dense(nominal_model_path)) {
  // TODO: add uncertainty set instantiation
}

void robust_linear_program::add_uncertainty_set(
    int constraint_id, std::unique_ptr<uncertainty_set> set) {
  robust_rows_.insert(constraint_id);
  uncertainty_sets_[constraint_id] = std::move(set);
}
