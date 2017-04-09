//
// Created by Christian Kroer on 4/03/17.
//

#include "./robust_linear_program.h"
#include <fstream>
#include <iostream>
#include <string>

robust_linear_program::robust_linear_program(std::string nominal_model_path)
    : nominal_model_path_(nominal_model_path),
      nominal_program_(nominal_model_path) {}

robust_linear_program(std::string model_path, uncertainty_type)
    : nominal_model_path_(nominal_model_path),
      nominal_program_(nominal_model_path) {
  // TODO: add uncertainty set instantiation
}

void add_uncertainty_set(int constraint_id, uncertainty_set set) {
  robust_rows_.push_back(constraint_id);
  nominal_rows_.erase(
      std::remove(nominal_rows_.begin(), nominal_rows_.end(), constraint_id),
      nominal_rows_.end());
  uncertainty_sets_.push_back(set);
}
