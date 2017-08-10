//
// Created by Christian Kroer on 4/04/17.
//

#ifndef SRC_MODEL_LINEAR_UNCERTAINTY_CONSTRAINT_H_
#define SRC_MODEL_LINEAR_UNCERTAINTY_CONSTRAINT_H_

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include "../domain/domain.h"
#include "./uncertainty_constraint.h"

class linear_uncertainty_constraint : public uncertainty_constraint {
 public:
  linear_uncertainty_constraint(
      int constraint_id, std::unique_ptr<domain> dom,
      sparse_vector_d nominal_coeffs, vector_d weights,
      std::vector<int> uncertainty_variable_ids, double rhs = 0,
      char sense = '<', std::vector<double> certain_variable_coefficient = {},
      std::vector<int> certain_variable_index = {},
      std::vector<std::string> certain_variable_name = {});

  uncertainty_constraint::function_type get_function_type() const override {
    return uncertainty_constraint::LINEAR;
  }
  int dimension() const override { return domain_->dimension(); }
  int get_constraint_id() const override { return constraint_id_; }
  double get_rhs() const { return rhs_; }
  std::pair<double, vector_d> maximizer(const vector_d current) const override;
  vector_d gradient(const vector_d &solution,
                    const vector_d &unc_vec) const override;
  vector_d gradient(const vector_d &solution) const;
  const std::vector<int> &uncertainty_variable_ids() const {
    return uncertainty_variable_ids_;
  }
  sparse_vector_d &nominal_coeffs() { return nominal_coeffs_; }
  const domain *get_domain() const override { return domain_.get(); };
  double violation_amount(const vector_d &solution,
                          const vector_d &constraint_params) const override;
  sparse_vector_d get_full_coeffs(vector_d uncertain_coeffs) const;
  double get_coeff(int uncertainty_id, double uncertainty_coeff) const;
  const vector_d &uncertain_nominal_coeffs() const {
    return uncertain_nominal_coeffs_;
  }

 private:
  int constraint_id_;
  std::unique_ptr<domain> domain_;
  sparse_vector_d nominal_coeffs_;
  vector_d weights_;
  std::vector<int> uncertainty_variable_ids_;
  double rhs_;
  char sense_;
  std::unordered_map<int, int> var_id_to_uncertainty_id_;
  // nominal coefficients for just uncertain variables
  vector_d uncertain_nominal_coeffs_;
};

#endif  // SRC_MODEL_LINEAR_UNCERTAINTY_CONSTRAINT_H_
