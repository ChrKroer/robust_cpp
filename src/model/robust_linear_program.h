//
// Created by Christian Kroer on 4/03/17.
//

#ifndef ROBUST_CPP_ROBUST_LINEAR_PROGRAM_H
#define ROBUST_CPP_ROBUST_LINEAR_PROGRAM_H

#include "./../basic_types.h"
#include "./nominal_program.h"
#include "./robust_program_dense.h"
#include "./uncertainty_constraint.h"
#include <unordered_map>
#include <unordered_set>

class robust_linear_program : public robust_program_dense {
public:
  // Only builds nominal problem from the given model file. Uncertainty must be
  // added separately
  explicit robust_linear_program(std::string nominal_model_path);
  // Adds random uncertainty sets of the given type
  robust_linear_program(std::string nominal_model_path,
                        uncertainty_constraint::constraint_type);
  ~robust_linear_program() {}

  int dimension() const override { return nominal_program_->dimension(); }
  int num_constraints() const override {
    return nominal_program_->num_constraints();
  }
  int num_uncertainty_constraints() const override { return robust_rows_.size(); }
  std::unordered_set<int>::const_iterator
  robust_constraints_begin() const override {
    return robust_rows_.cbegin();
  }
  std::unordered_set<int>::const_iterator
  robust_constraints_end() const override {
    return robust_rows_.cend();
  }
  const uncertainty_constraint &get_uncertainty_constraint(int id) const override {
    return *uncertainty_constraints_.at(id);
  }
  const nominal_program &get_nominal_program() const override {
    return *nominal_program_;
  }

  std::string nominal_model_path() const override {
    return nominal_model_path_;
  }
  void add_uncertainty_constraint(int constraint_id,
                           std::unique_ptr<uncertainty_constraint> set);

private:
  std::unordered_set<int> robust_rows_;
  std::unordered_map<int, std::unique_ptr<uncertainty_constraint>> uncertainty_constraints_;

  std::string nominal_model_path_;
  std::unique_ptr<nominal_program> nominal_program_;
};

#endif // ROBUST_CPP_ROBUST_LINEAR_PROGRAM_H
