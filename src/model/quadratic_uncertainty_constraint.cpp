//
// Created by Christian Kroer on 5/01/17.
//
#include <chrono>

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
    const vector_d nominal_solution) const {
  const vector_d lin =
      2 * get_linear_uncertainty_coefficients(nominal_solution);
  const matrix_d Y = get_pairwise_uncertainty_quadratic(nominal_solution);
  auto trs_sol_pair = trs_subproblem_solution(lin, Y);
  double trs_obj =
      std::get<0>(trs_sol_pair) - rhs_ +
      (base_matrix_ * get_nominal_active_variables(nominal_solution))
          .squaredNorm();
  for (int i = 0; i < certain_variable_coefficient_.size(); i++) {
    trs_obj += certain_variable_coefficient_[i] *
               nominal_solution(certain_variable_index_[i]);
  }
  return std::make_pair(trs_obj, std::get<1>(trs_sol_pair));
}

vector_d quadratic_uncertainty_constraint::gradient(
    const vector_d &solution, const vector_d &unc_vec) const {
  const vector_d lin = get_linear_uncertainty_coefficients(solution);
  const matrix_d Y = get_pairwise_uncertainty_quadratic(solution);

  Eigen::SelfAdjointEigenSolver<matrix_d> es(unc_vec.size());
  es.compute(Y);
  double max_eigenval = es.eigenvalues()(unc_vec.size() - 1);
  const vector_d max_eigenvec = es.eigenvectors().col(unc_vec.size() - 1);
  const double inner_prod = max_eigenvec.dot(lin);

  const vector_d y_unc = Y * unc_vec;
  const vector_d lambda_unc = max_eigenval * unc_vec;
  const vector_d return_val = lin + y_unc - lambda_unc;

  return 2 * (lin + Y * unc_vec - max_eigenval * unc_vec);
}

void quadratic_uncertainty_constraint::push_to_boundary(
    vector_d *v, const vector_d &x) const {
  if (std::abs((*v).norm()) - 1 > 1e-8) return;
  const matrix_d Y = get_pairwise_uncertainty_quadratic(x);

  Eigen::SelfAdjointEigenSolver<matrix_d> es((*v).size());
  es.compute(Y);
  vector_d max_eigenvec = es.eigenvectors().col((*v).size() - 1);
  double low = 0.0;
  double high = 2.0;
  double mid = low + (high - low) / 2;
  while (std::abs((*v + mid * max_eigenvec).norm() - 1) > 1e-12) {
    if ((*v + mid * max_eigenvec).norm() < 1.0) {
      low = low + (high - low) / 2;
    } else {
      high = low + (high - low) / 2;
    }
    mid = low + (high - low) / 2;
  }

  *v += mid * max_eigenvec;
}

double quadratic_uncertainty_constraint::violation_amount(
    const vector_d &solution, const vector_d &unc_vec) const {
  vector_d nominal_subset = get_nominal_active_variables(solution);
  matrix_d m = get_matrix_instantiation(unc_vec);
  double violation_amount = (m * nominal_subset).squaredNorm() - rhs_;
  for (int i = 0; i < certain_variable_coefficient_.size(); i++) {
    violation_amount +=
        certain_variable_coefficient_[i] * solution(certain_variable_index_[i]);
  }
  return violation_amount;
}

double quadratic_uncertainty_constraint::violation_amount_concavified(
    const vector_d &solution, const vector_d &unc_vec) const {
  const matrix_d Y = get_pairwise_uncertainty_quadratic(solution);
  Eigen::SelfAdjointEigenSolver<matrix_d> es(unc_vec.size());
  es.compute(Y);
  double max_eigenval = es.eigenvalues()(unc_vec.size() - 1);

  vector_d nominal_subset = get_nominal_active_variables(solution);
  matrix_d m = get_matrix_instantiation(unc_vec);
  double violation_amount = (m * nominal_subset).squaredNorm() - rhs_ +
                            max_eigenval * (1 - unc_vec.norm());
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
  std::unordered_map<int, vector_d> x_P;
  for (int i = 0; i < domain_->dimension(); i++) {
    const matrix_d &m = uncertain_matrices_[i];
    x_P[i] = m * nominal_subset;
  }
  matrix_d Y = matrix_d::Zero(domain_->dimension(), domain_->dimension());
  for (int i = 0; i < domain_->dimension(); i++) {
    for (int j = 0; j < domain_->dimension(); j++) {
      Y(i, j) = x_P[i].dot(x_P[j]);
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
    const vector_d &lin, const matrix_d &Y) const {
  trust_region tr(lin, Y);
  tr.optimize();
  vector_d sol = tr.get_solution();
  return std::make_pair(tr.get_objective(), sol);
}
