//
// Created by Christian Kroer on 5/01/17.
//

#include "./../logging.h"
#include "./../solver/trust_region.h"
#include "./quadratic_uncertainty_constraint.h"
#include "Eigen/Eigenvalues"

quadratic_uncertainty_constraint::quadratic_uncertainty_constraint(
    std::string constraint_name, std::unique_ptr<domain> dom,
    matrix_d base_matrix, std::vector<int> nominal_indices,
    std::vector<matrix_d> uncertain_matrices, double rhs,
    std::vector<double> certain_variable_coefficient,
    std::vector<int> certain_variable_index,
    std::vector<std::string> certain_variable_name)
    : domain_(std::move(dom)),
      base_matrix_(base_matrix),
      nominal_indices_(nominal_indices),
      uncertain_matrices_(uncertain_matrices),
      rhs_(rhs) {
  name_ = constraint_name;
  certain_variable_coefficient_ = certain_variable_coefficient;
  certain_variable_index_ = certain_variable_index;
  certain_variable_name_ = certain_variable_name;
}

std::pair<double, vector_d> quadratic_uncertainty_constraint::maximizer(
    const vector_d current) const {
  return trs_subproblem_solution(current);
  // std::pair<double, vector_d> trs = trs_subproblem_solution(current);
  // double violation = violation_amount(current, trs.second);
  // return std::make_pair(violation, trs.second);
}

vector_d quadratic_uncertainty_constraint::gradient(
    const vector_d &solution, const vector_d &unc_vec) const {
  const vector_d lin = get_linear_uncertainty_coefficients(solution);
  const matrix_d Y = get_pairwise_uncertainty_quadratic(solution);

  Eigen::SelfAdjointEigenSolver<matrix_d> es(unc_vec.size());
  es.compute(Y);
  double max_eigenval = es.eigenvalues()(unc_vec.size() - 1);

  return 2 * (lin + Y * unc_vec - max_eigenval * unc_vec);
}

double quadratic_uncertainty_constraint::violation_amount(
    const vector_d &solution, const vector_d &constraint_params) const {
  vector_d nominal_subset = get_nominal_active_variables(solution);
  matrix_d m = get_matrix_instantiation(constraint_params);
  double violation_amount = (m * nominal_subset).squaredNorm() - rhs_;
  double certain_amount = 0;
  for (int i = 0; i < certain_variable_coefficient_.size(); i++) {
    violation_amount +=
        certain_variable_coefficient_[i] * solution(certain_variable_index_[i]);
    certain_amount +=
        certain_variable_coefficient_[i] * solution(certain_variable_index_[i]);
  }
  return violation_amount;
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
  const vector_d lin =
      2 * get_linear_uncertainty_coefficients(nominal_solution);
  const matrix_d Y = get_pairwise_uncertainty_quadratic(nominal_solution);
  // sparse_matrix_d sparse_Y = Y.sparseView();
  trust_region tr(lin, Y);
  tr.optimize();
  vector_d sol = tr.get_solution();
  double trs_obj =
      tr.get_objective() - rhs_ +
      (base_matrix_ * get_nominal_active_variables(nominal_solution))
          .squaredNorm();
  double trs_grb_obj =
      tr.get_grb_objective() - rhs_ +
      (base_matrix_ * get_nominal_active_variables(nominal_solution))
          .squaredNorm();
  for (int i = 0; i < certain_variable_coefficient_.size(); i++) {
    trs_obj += certain_variable_coefficient_[i] *
               nominal_solution(certain_variable_index_[i]);
    trs_grb_obj += certain_variable_coefficient_[i] *
                   nominal_solution(certain_variable_index_[i]);
  }
  // logger->debug("Nom sol: {}", eigen_to_string(nominal_solution));
  // logger->debug("trs obj: {}, trs grb obj: {}, violation amount: {}",
  // trs_obj,
  //               trs_grb_obj, violation_amount(nominal_solution, sol));
  // logger->debug("λ(||u|| - 1): {}, norm: {}, eigenval: {}",
  //               tr.get_max_eigenval() * (sol.norm() - 1), sol.norm(),
  //               tr.get_max_eigenval());

  return std::make_pair(trs_obj, sol);
}
