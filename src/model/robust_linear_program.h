//
// Created by Christian Kroer on 4/03/17.
//

#ifndef ROBUST_CPP_ROBUST_LINEAR_PROGRAM_H
#define ROBUST_CPP_ROBUST_LINEAR_PROGRAM_H

#include "./../basic_types.h"
#include "./nominal_program.h"
#include "./robust_program.h"
#include "./uncertainty_set.h"
#include <unordered_map>
#include <unordered_set>

class robust_linear_program : public robust_program {
public:
  // Only builds nominal problem from the given model file. Uncertainty must be
  // added separately
  explicit robust_linear_program(std::string nominal_model_path);
  // Adds random uncertainty sets of the given type
  robust_linear_program(std::string nominal_model_path,
                        uncertainty_set::uncertainty_type);
  ~robust_linear_program() {}

  void add_uncertainty_set(int constraint_id, uncertainty_set set);

  int dimension() const { return nominal_program_->dimension(); }
  int num_constraints() const { return nominal_program_->num_constraints(); }
  std::string nominal_model_path() const { return nominal_model_path_; }
  int num_uncertainty_sets() const { return robust_rows_.size(); }
  std::unordered_set<int>::const_iterator robust_constraints_begin() const {
    return robust_rows_.cbegin();
  }
  std::unordered_set<int>::const_iterator robust_constraints_end() const {
    return robust_rows_.cend();
  }
  const uncertainty_set &get_uncertainty_set(int id) const {
    return *uncertainty_sets_.at(id);
  }
  constraint_type get_constraint_type(int id) const {
    return robust_program::linear;
  }
  const nominal_program &get_nominal_program() const { return *nominal_program_; }

  void
  add_uncertainty_set(int constraint_id,
                      std::unique_ptr<uncertainty_set::uncertainty_set> set);

private:
  std::unordered_set<int> robust_rows_;
  std::unordered_map<int, std::unique_ptr<uncertainty_set>> uncertainty_sets_;

  std::string nominal_model_path_;
  std::unique_ptr<nominal_program> nominal_program_;
};

#endif // ROBUST_CPP_ROBUST_LINEAR_PROGRAM_H
