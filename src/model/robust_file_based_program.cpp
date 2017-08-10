//
// Created by Christian Kroer on 4/03/17.
//

#include "./robust_file_based_program.h"
#include "./nominal_program_dense.h"
#include "./robust_reader.h"
#include <iostream>

robust_file_based_program::robust_file_based_program(
    std::string nominal_model_path)
    : nominal_model_path_(nominal_model_path),
      nominal_program_(new nominal_program_dense(nominal_model_path)) {}

robust_file_based_program::robust_file_based_program(
    std::string nominal_model_path, std::string robust_file_path)
    : nominal_model_path_(nominal_model_path),
      nominal_program_(new nominal_program_dense(nominal_model_path)) {
  robust_reader r(nominal_model_path, robust_file_path);
  while (r.has_next()) {
    std::unique_ptr<uncertainty_constraint> c = r.next_uncertainty_constraint();
    add_uncertainty_constraint(std::move(c));
  }
}

void robust_file_based_program::add_uncertainty_constraint(
    std::unique_ptr<uncertainty_constraint> constraint) {
  std::string name = constraint->get_constraint_name();
  robust_rows_.insert(name);
  uncertainty_constraints_[name] = std::move(constraint);
}
