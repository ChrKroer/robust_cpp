//
// Created by Christian Kroer on 4/03/17.
//

#ifndef ROBUST_CPP_ROBUST_FILE_BASED_PROGRAM_H
#define ROBUST_CPP_ROBUST_FILE_BASED_PROGRAM_H

#include "./../basic_types.h"
#include "./nominal_program.h"
#include "./robust_program_dense.h"
#include "./uncertainty_constraint.h"
#include <memory>
#include <unordered_map>
#include <unordered_set>

class robust_file_based_program : public robust_program_dense {
public:
  // Only builds nominal problem from the given model file. Uncertainty must be
  // added separately
  explicit robust_file_based_program(std::string nominal_model_path);
  robust_file_based_program(std::string nominal_model_path,
                            std::string robust_file_path);

  int dimension() const override { return nominal_program_->dimension(); }
  int num_constraints() const override {
    return nominal_program_->num_constraints();
  }
  int num_uncertainty_constraints() const override {
    return robust_rows_.size();
  }
  std::unordered_set<std::string>::const_iterator
  robust_constraints_begin() const override {
    return robust_rows_.cbegin();
  }
  std::unordered_set<std::string>::const_iterator
  robust_constraints_end() const override {
    return robust_rows_.cend();
  }
  const uncertainty_constraint &
  get_uncertainty_constraint(std::string name) const override {
    return *uncertainty_constraints_.at(name);
  };
  
  const nominal_program &get_nominal_program() const override {
    return *nominal_program_;
  }

  std::string nominal_model_path() const override {
    return nominal_model_path_;
  }
  void add_uncertainty_constraint(std::unique_ptr<uncertainty_constraint> set);

private:
  std::unordered_set<std::string> robust_rows_;
  std::unordered_map<std::string, std::unique_ptr<uncertainty_constraint>>
      uncertainty_constraints_;

  std::string nominal_model_path_;
  std::unique_ptr<nominal_program> nominal_program_;
};

#endif // ROBUST_CPP_ROBUST_FILE_BASED_PROGRAM_H
