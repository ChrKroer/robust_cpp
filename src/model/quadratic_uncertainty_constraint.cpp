//
// Created by Christian Kroer on 5/01/17.
//

#include "./quadratic_uncertainty_constraint.h"
#include "./../logging.h"
#include "./../solver/trust_region.h"
#include "Eigen/Eigenvalues"

quadratic_uncertainty_constraint::quadratic_uncertainty_constraint(
    int constraint_id, int dimension, std::unique_ptr<domain> dom,
    matrix_d base_matrix, std::vector<int> nominal_indices,
    std::vector<matrix_d> uncertain_matrices, double rhs)
    : constraint_id_(constraint_id), domain_(std::move(dom)),
      base_matrix_(base_matrix), nominal_indices_(nominal_indices),
      uncertain_matrices_(uncertain_matrices), rhs_(rhs) {}

std::pair<double, vector_d>
quadratic_uncertainty_constraint::maximizer(const vector_d current) const {
  std::pair<double, vector_d> trs = trs_subproblem_solution(current);
  double violation = violation_amount(current, trs.second);
  return std::make_pair(violation, trs.second);
};

vector_d
quadratic_uncertainty_constraint::gradient(const vector_d &current) const {
  vector_d g = vector_d(domain_->dimension());
  return g;
}

double quadratic_uncertainty_constraint::violation_amount(
    const vector_d &solution, const vector_d &constraint_params) const {
  vector_d nominal_subset = get_nominal_active_variables(solution);
  double val = 0;
  val += (base_matrix_ * nominal_subset).squaredNorm();
  for (int i = 0; i < constraint_params.size(); i++) {
    const matrix_d &m = uncertain_matrices_[i];
    val += (m * nominal_subset).dot(base_matrix_ * nominal_subset) *
           constraint_params(i);
  }

  return val - rhs_;
}

vector_d quadratic_uncertainty_constraint::get_nominal_active_variables(
    const vector_d nominal_solution) const {
  vector_d v(nominal_indices_.size());
  for (int i = 0; i < nominal_indices_.size(); i++) {
    v(i) = nominal_solution(nominal_indices_[i]);
  }
  return v;
}

matrix_d quadratic_uncertainty_constraint::get_matrix_instantiation(
    const vector_d uncertain_solution) {
  matrix_d m = base_matrix_;
  for (int i = 0; i < dimension(); i++) {
    m += uncertain_matrices_[i] * uncertain_solution(i);
  }
  return m;
}

matrix_d quadratic_uncertainty_constraint::get_pairwise_uncertainty_quadratic(
    const vector_d &nominal_solution) const {
  vector_d nominal_subset = get_nominal_active_variables(nominal_solution);
  matrix_d Y(domain_->dimension(), domain_->dimension());
  for (int i = 0; i < domain_->dimension(); i++) {
    for (int j = 0; j < domain_->dimension(); j++) {
      const matrix_d &m = uncertain_matrices_[i];
      Y(i, j) = (m * nominal_subset).squaredNorm();
    }
  }
  return Y;
}

vector_d quadratic_uncertainty_constraint::get_linear_uncertainty_coefficients(
    const vector_d &nominal_solution) const {
  vector_d nominal_subset = get_nominal_active_variables(nominal_solution);
  vector_d lin(dimension());
  for (int i = 0; i < dimension(); i++) {
    const matrix_d &m = uncertain_matrices_[i];
    lin(i) += (m * nominal_subset).dot(base_matrix_ * nominal_subset);
  }
  return lin;
}

std::pair<double, vector_d>
quadratic_uncertainty_constraint::trs_subproblem_solution(
    const vector_d &nominal_solution) const {
  vector_d lin = get_linear_uncertainty_coefficients(nominal_solution);
  matrix_d Y = get_pairwise_uncertainty_quadratic(nominal_solution);

  trust_region tr(lin, Y.sparseView());
  std::cout << std::endl;
  tr.optimize();
  vector_d sol = tr.get_solution();

  return std::make_pair(tr.get_objective(), sol);
}
