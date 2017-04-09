//
// Created by Christian Kroer on 4/03/17.
//

#ifndef ROBUST_CPP_ROBUST_LINEAR_PROGRAM_H
#define ROBUST_CPP_ROBUST_LINEAR_PROGRAM_H

#include "./../basic_types.h"
#include "./robust_program.h"
#include "./uncertainty_set.h"
#include <vector>

class robust_linear_program : public robust_program {
public:
  // Only builds nominal problem from the given model file. Uncertainty must be
  // added separately
  explicit robust_linear_program(std::string nominal_model_path);
  // Adds random uncertainty sets of the given type
  robust_linear_program(std::string nominal_model_path, uncertainty_type);
  ~robust_linear_program() {}

  void add_uncertainty_set(int constraint_id, uncertainty_set set);

  const std::vector<int> &get_robust_rows_() const { return robust_rows_; }
  const std::vector<int> &get_nominal_rows_() const { return nominal_rows_; }
  const std::vector<uncertainty_set> &get_uncertainty_sets_() const {
    return uncertainty_sets_;
  }

  const int dimension() { return nominal_program_.dimension(); }
  const int num_constraints() { return nominal_program_.num_constraints(); }
  const std::string model_path() { return model_path_; }
  const int num_uncertainty_sets() { return robust_rows_.size(); }
  const uncertainty_set &uncertainty_set(int id) {
    return uncertainty_sets_[id];
  }

private:
  std::vector<int> robust_rows_;
  std::vector<int> nominal_rows_;
  std::vector<uncertainty_set> uncertainty_sets_;

  std::string nominal_model_path_;
  nominal_program nominal_program_;
};

#endif // ROBUST_CPP_ROBUST_LINEAR_PROGRAM_H
