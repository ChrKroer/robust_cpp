//
// Created by Christian Kroer on 5/01/17.
//

#ifndef SRC_MODEL_QUADRATIC_UNCERTAINTY_CONSTRAINT_H_
#define SRC_MODEL_QUADRATIC_UNCERTAINTY_CONSTRAINT_H_

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include "../domain/domain.h"
#include "./uncertainty_constraint.h"

class quadratic_uncertainty_constraint : public uncertainty_constraint {
 public:
  quadratic_uncertainty_constraint(
      std::string constraint_name, std::unique_ptr<domain> dom,
      matrix_d base_matrix_, std::vector<int> nominal_indices,
      std::vector<matrix_d> uncertain_matrices, double rhs = 0,
      std::vector<double> certain_variable_coefficient = {},
      std::vector<int> certain_variable_index = {},
      std::vector<std::string> certain_variable_name = {});

  uncertainty_constraint::function_type get_function_type() const override {
    return uncertainty_constraint::QUADRATIC;
  }
  int dimension() const override { return domain_->dimension(); }
  int get_nominal_id(int unc_id) const { return nominal_indices_[unc_id]; }
  double get_rhs() const { return rhs_; }
  std::string get_name() const { return name_; }
  /**
   * @brief Computes the maximizing uncertainty coefficients for the given
   * nominal solution. Also returns the constraint value corresponding to the
   * resulting pair.
   */
  std::pair<double, vector_d> maximizer(
      const vector_d nominal_solution) const override;
  matrix_d get_pairwise_uncertainty_quadratic(
      const vector_d &nominal_solution) const;
  vector_d get_linear_uncertainty_coefficients(
      const vector_d &nominal_solution) const;
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
  void push_to_boundary(vector_d *v, const vector_d &x) const override;
  double violation_amount(const vector_d &solution,
                          const vector_d &unc_vec) const override;
  double violation_amount_concavified(const vector_d &solution,
                                      const vector_d &unc_vec) const;
  const matrix_d &base_matrix() const { return base_matrix_; }
  const std::vector<matrix_d> &uncertain_matrices() const {
    return uncertain_matrices_;
  }
  matrix_d get_matrix_instantiation(const vector_d uncertain_solution) const;

  /**
   * @brief Computes the solution to the trust-region subproblem associated
   * with the given linear coefficients and matrix.
   */
  std::pair<double, vector_d> trs_subproblem_solution(const vector_d &lin,
                                                      const matrix_d &Y) const;

 private:
  std::unique_ptr<domain> domain_;
  const matrix_d base_matrix_;
  std::vector<int> nominal_indices_;
  const std::vector<matrix_d> uncertain_matrices_;
  double rhs_;

  vector_d get_nominal_active_variables(const vector_d nominal_solution) const;
};

#endif  // SRC_MODEL_QUADRATIC_UNCERTAINTY_CONSTRAINT_H_
