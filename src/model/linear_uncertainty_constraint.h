//
// Created by Christian Kroer on 4/04/17.
//

#ifndef ROBUST_CPP_LINEAR_UNCERTAINTY_CONSTRAINT_H
#define ROBUST_CPP_LINEAR_UNCERTAINTY_CONSTRAINT_H

#include "../domain/domain.h"
#include "./uncertainty_constraint.h"
#include <vector>
#include <unordered_map>

class linear_uncertainty_constraint : public uncertainty_constraint {
public:
  linear_uncertainty_constraint(int constraint_id, std::unique_ptr<domain> dom,
                                std::vector<std::pair<int,double>> nominal_coeffs,
                                std::vector<int> uncertainty_variable_ids, double rhs = 0, char sense = '<');
  ~linear_uncertainty_constraint() {}
  uncertainty_constraint::function_type get_function_type() const override {
    return uncertainty_constraint::LINEAR;
  }
  int dimension() const override { return domain_->dimension(); }
  int get_constraint_id() const override { return constraint_id_; }
  double get_rhs() const { return rhs_; }
  std::pair<double, vector_d> maximizer(const vector_d current) const override;
  vector_d gradient(const vector_d &current) const override;
  const std::vector<int> &uncertainty_variable_ids() const override {
    return uncertainty_variable_ids_;
  }
  const std::vector<std::pair<int,double>> &nominal_coeffs() const {
    return nominal_coeffs_;
  }
  const domain *get_domain() const override { return domain_.get(); };
  double violation_amount(const vector_d &solution,
                          const vector_d &constraint_params) const override;
  std::vector<std::pair<int, double>> get_full_coeffs(vector_d coeffs) const;

private:
  std::unique_ptr<domain> domain_;
  double rhs_;
  char sense_;
  std::vector<int> uncertainty_variable_ids_;
  std::unordered_map<int, int> var_id_to_uncertainty_id_;
  std::vector<std::pair<int,double>> nominal_coeffs_;
  int constraint_id_;
};

#endif // ROBUST_CPP_LINEAR_UNCERTAINTY_CONSTRAINT_H
