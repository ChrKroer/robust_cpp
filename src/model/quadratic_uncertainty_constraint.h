//
// Created by Christian Kroer on 5/01/17.
//

#ifndef ROBUST_CPP_QUADRATIC_UNCERTAINTY_CONSTRAINT_H
#define ROBUST_CPP_QUADRATIC_UNCERTAINTY_CONSTRAINT_H

#include <memory>
#include <unordered_map>
#include <vector>
#include "../domain/domain.h"
#include "./uncertainty_constraint.h"

class quadratic_uncertainty_constraint : public uncertainty_constraint {
 public:
  quadratic_uncertainty_constraint(
      int constraint_id, std::unique_ptr<domain> dom, matrix_d base_matrix_,
      std::vector<int> nominal_indices,
      std::vector<matrix_d> uncertain_matrices,
      std::vector<double> certain_variable_coefficient = {},
      std::vector<std::string> certain_variable_name = {}, double rhs = 0);

  uncertainty_constraint::function_type get_function_type() const override {
    return uncertainty_constraint::QUADRATIC;
  }
  int dimension() const override { return domain_->dimension(); }
  int get_constraint_id() const override { return constraint_id_; }
  int get_nominal_id(int unc_id) const { return nominal_indices_[unc_id]; }
  double get_rhs() const { return rhs_; }
  std::pair<double, vector_d> maximizer(const vector_d current) const override;
  /**
   * @brief Provides the gradient of the uncertainty constraints LHS wrt.
   * unc_vec.
   *
   * `solution` should be a potential, not necessarily feasible, solution to the
   * model that the constraint belongs to, and unc_vec should be a vector from
   * the uncertainty set at this constraint.
   */
  vector_d gradient(const vector_d &solution,
                    const vector_d &unc_vec) const override;
  const domain *get_domain() const override { return domain_.get(); };
  double violation_amount(const vector_d &solution,
                          const vector_d &constraint_params) const override;
  const matrix_d &base_matrix() const { return base_matrix_; };
  const std::vector<matrix_d> &uncertain_matrices() const {
    return uncertain_matrices_;
  };
  matrix_d get_matrix_instantiation(const vector_d uncertain_solution) const;

 private:
  int constraint_id_;
  std::unique_ptr<domain> domain_;
  matrix_d base_matrix_;
  std::vector<int> nominal_indices_;
  std::vector<matrix_d> uncertain_matrices_;
  double rhs_;

  vector_d get_nominal_active_variables(const vector_d nominal_solution) const;
  matrix_d get_pairwise_uncertainty_quadratic(
      const vector_d &nominal_solution) const;
  vector_d get_linear_uncertainty_coefficients(
      const vector_d &nominal_solution) const;
  std::pair<double, vector_d> trs_subproblem_solution(
      const vector_d &nominal_solution) const;
};

#endif  // ROBUST_CPP_QUADRATIC_UNCERTAINTY_CONSTRAINT_H
