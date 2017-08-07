//
// Created by Christian Kroer on 5/01/17.
//

#include "./../logging.h"
#include "./../solver/trust_region.h"
#include "./quadratic_uncertainty_constraint.h"
#include "Eigen/Eigenvalues"

quadratic_uncertainty_constraint::quadratic_uncertainty_constraint(
    int constraint_id, std::unique_ptr<domain> dom, matrix_d base_matrix,
    std::vector<int> nominal_indices, std::vector<matrix_d> uncertain_matrices,
    std::vector<double> certain_variable_coefficient,
    std::vector<std::string> certain_variable_name, double rhs)
    : constraint_id_(constraint_id),
      domain_(std::move(dom)),
      base_matrix_(base_matrix),
      nominal_indices_(nominal_indices),
      uncertain_matrices_(uncertain_matrices),
      rhs_(rhs) {
  certain_variable_name_ = certain_variable_name;
  certain_variable_coefficient_ = certain_variable_coefficient;
}

std::pair<double, vector_d> quadratic_uncertainty_constraint::maximizer(
    const vector_d current) const {
  std::pair<double, vector_d> trs = trs_subproblem_solution(current);
  double violation = violation_amount(current, trs.second);
  return std::make_pair(violation, trs.second);
}

vector_d quadratic_uncertainty_constraint::gradient(
    const vector_d &current) const {
  vector_d g = vector_d(domain_->dimension());
  // TODO: write this
  return g;
}

double quadratic_uncertainty_constraint::violation_amount(
    const vector_d &solution, const vector_d &constraint_params) const {
  vector_d nominal_subset = get_nominal_active_variables(solution);
  matrix_d m = get_matrix_instantiation(constraint_params);
  // TODO(chrkroer): add certain coefficients here
  return (m * nominal_subset).squaredNorm() - rhs_;
}

// removes inactive entries nominal solution
vector_d quadratic_uncertainty_constraint::get_nominal_active_variables(
    const vector_d nominal_solution) const {
  vector_d v(nominal_indices_.size());
  for (int i = 0; i < nominal_indices_.size(); i++) {
    v(i) = nominal_solution(nominal_indices_[i]);
  }
  return v;
}

matrix_d quadratic_uncertainty_constraint::get_matrix_instantiation(
    const vector_d uncertain_solution) const {
  matrix_d m = base_matrix_;
  for (int i = 0; i < dimension(); i++) {
    m += uncertain_matrices_[i] * uncertain_solution(i);
  }
  return m;
}

// Y(x) := \{x^\top P_j^\top P_{j'} x\}_{j,j' \in [k]} \in R^{k x k}
matrix_d quadratic_uncertainty_constraint::get_pairwise_uncertainty_quadratic(
    const vector_d &nominal_solution) const {
  const vector_d nominal_subset =
      get_nominal_active_variables(nominal_solution);
  matrix_d Y = matrix_d::Zero(domain_->dimension(), domain_->dimension());
  for (int i = 0; i < domain_->dimension(); i++) {
    for (int j = 0; j < domain_->dimension(); j++) {
      const matrix_d &m = uncertain_matrices_[i];
      const matrix_d &n = uncertain_matrices_[j];
      Y(i, j) = (m * nominal_subset).dot(n * nominal_subset);
    }
  }
  return Y;
}

// b(x) := \{x^\top A_0^\top P_{j} x\}_{j \in [k]} \in R^k
vector_d quadratic_uncertainty_constraint::get_linear_uncertainty_coefficients(
    const vector_d &nominal_solution) const {
  const vector_d nominal_subset =
      get_nominal_active_variables(nominal_solution);
  vector_d lin = vector_d::Zero(dimension());
  for (int i = 0; i < dimension(); i++) {
    const matrix_d &m = uncertain_matrices_[i];
    lin(i) += (m * nominal_subset).dot(base_matrix_ * nominal_subset);
  }
  return lin;
}

std::pair<double, vector_d>
quadratic_uncertainty_constraint::trs_subproblem_solution(
    const vector_d &nominal_solution) const {
  const vector_d lin = get_linear_uncertainty_coefficients(nominal_solution);
  const matrix_d Y = get_pairwise_uncertainty_quadratic(nominal_solution);
  // sparse_matrix_d sparse_Y = Y.sparseView();
  trust_region tr(lin, Y);
  tr.optimize();
  vector_d sol = tr.get_solution();

  return std::make_pair(tr.get_objective(), sol);
}
