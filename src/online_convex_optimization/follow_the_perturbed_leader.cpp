//
// Created by Christian Kroer on 3/30/17.
//

#include <math.h>
#include "./../logging.h"
#include "./follow_the_perturbed_leader.h"

follow_the_perturbed_leader::follow_the_perturbed_leader(
    const domain* dom, const double stepsize_scalar,
    const quadratic_uncertainty_constraint& unc_constraint)
    : domain_(dom),
      stepsize_scalar_(stepsize_scalar),
      uncertainty_constraint_(unc_constraint) {
  init();
}

void follow_the_perturbed_leader::init() {
  averaged_matrix_ =
      matrix_d::Zero(get_domain().dimension(), get_domain().dimension());
  averaged_vector_ = vector_d::Zero(get_domain().dimension());
  iterate_ = get_domain().get_prox().center();
  // sqrt(n) l2-norm-ball diameter + n*sqrt(n) Frobenius norm on outer product
  diameter_ = (1 + domain_->dimension()) * sqrt(domain_->dimension());
}

void follow_the_perturbed_leader::receive_gradient(vector_d nominal_solution) {
  num_iterations_++;
  matrix_d matrix_gradient =
      uncertainty_constraint_.get_pairwise_uncertainty_quadratic(
          nominal_solution);
  vector_d vector_gradient =
      uncertainty_constraint_.get_linear_uncertainty_coefficients(
          nominal_solution);
  update_gradient_max(matrix_gradient, vector_gradient);
  averaged_matrix_ += matrix_gradient;
  averaged_vector_ += vector_gradient;
  const matrix_d Y = noisy_matrix();
  const vector_d lin = noisy_vector();
  std::tuple<double, vector_d> tup =
      uncertainty_constraint_.trs_subproblem_solution(lin, Y);
  iterate_ = std::get<1>(tup);
}

matrix_d follow_the_perturbed_leader::noisy_matrix() const {
  int rows = averaged_matrix_.rows();
  int cols = averaged_matrix_.cols();
  matrix_d uniform_matrix = matrix_d::Random(rows, cols);
  matrix_d uniform_symmetric = uniform_matrix.selfadjointView<Eigen::Upper>();
  uniform_symmetric += matrix_d::Constant(rows, cols, 1);
  uniform_symmetric.diagonal() *= 2;
  uniform_symmetric *=
      sqrt((f_max_ * gradient_max_ * num_iterations_) / diameter_);
  // logger->debug("perturbation:");
  // std::cout << uniform_symmetric << std::endl;
  // logger->debug("perturbed M:");
  // std::cout << averaged_matrix_ + uniform_symmetric << std::endl;
  return averaged_matrix_ + uniform_symmetric;
}

vector_d follow_the_perturbed_leader::noisy_vector() const {
  int size = averaged_vector_.size();
  vector_d uniform_vector = vector_d::Random(size);
  uniform_vector += vector_d::Constant(size, uniform_vector.minCoeff());
  uniform_vector *= 0.5 / stepsize_scalar_;
  return averaged_vector_ + uniform_vector;
}

void follow_the_perturbed_leader::update_gradient_max(const matrix_d& Y,
                                                      const vector_d& lin) {
  double new_val = lin.norm() + Y.norm();
  gradient_max_ = std::max(new_val, gradient_max_);
}
