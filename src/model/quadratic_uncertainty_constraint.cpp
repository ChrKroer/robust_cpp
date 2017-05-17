//
// Created by Christian Kroer on 5/01/17.
//

#include "./quadratic_uncertainty_constraint.h"
#include "./../logging.h"
#include "./../solver/trust_region.h"
#include "Eigen/Eigenvalues"

quadratic_uncertainty_constraint::quadratic_uncertainty_constraint(
    int constraint_id, int dimension, std::unique_ptr<domain> dom,
    matrix_d base_matrix, std::vector<matrix_d> uncertain_matrices, double rhs)
    : constraint_id_(constraint_id), domain_(std::move(dom)),
      base_matrix_(base_matrix), uncertain_matrices_(uncertain_matrices),
      rhs_(rhs) {}

std::pair<double, vector_d>
quadratic_uncertainty_constraint::maximizer(const vector_d current) const {
  vector_d g = gradient(current);
  return domain_->support(g);
};

vector_d
quadratic_uncertainty_constraint::gradient(const vector_d &current) const {
  vector_d g = vector_d(domain_->dimension());
  return g;
}

double quadratic_uncertainty_constraint::violation_amount(
    const vector_d &solution, const vector_d &constraint_params) const {
  double val = 0;
  val += (base_matrix_ * solution).squaredNorm();
  for (int i = 0; i < constraint_params.size(); i++) {
    const matrix_d &m = uncertain_matrices_[i];
    val += (m * solution).dot(base_matrix_ * solution) * constraint_params(i);
  }

  return val - rhs_;
}

matrix_d quadratic_uncertainty_constraint::get_pairwise_uncertainty_quadratic(
    const vector_d &nominal_solution) const {
  matrix_d Y(domain_->dimension(), domain_->dimension());
  for (int i = 0; i < domain_->dimension(); i++) {
    for (int j = 0; j < domain_->dimension(); j++) {
      const matrix_d &m = uncertain_matrices_[i];
      Y(i, j) = (m * nominal_solution).squaredNorm();
    }
  }
  return Y;
}

vector_d quadratic_uncertainty_constraint::get_linear_uncertainty_coefficients(
    const vector_d &nominal_solution) const {
  vector_d lin = 2 * base_matrix_ * nominal_solution;
  for (int i = 0; i < domain_->dimension(); i++) {
    const matrix_d &m = uncertain_matrices_[i];
    lin(i) += (m * nominal_solution).dot(base_matrix_ * nominal_solution);
  }
  return lin;
}

std::pair<double, vector_d>
quadratic_uncertainty_constraint::trs_subproblem_solution(
    const vector_d &nominal_solution) const {
  vector_d lin = get_linear_uncertainty_coefficients(nominal_solution);
  matrix_d Y = get_pairwise_uncertainty_quadratic(nominal_solution);

  trust_region tr(lin, Y.sparseView());
  tr.optimize();
  vector_d sol = tr.get_solution();

  return std::make_pair(tr.get_objective(), sol);
}
