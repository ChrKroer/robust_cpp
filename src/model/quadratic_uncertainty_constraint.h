//
// Created by Christian Kroer on 5/01/17.
//

#ifndef ROBUST_CPP_QUADRATIC_UNCERTAINTY_CONSTRAINT_H
#define ROBUST_CPP_QUADRATIC_UNCERTAINTY_CONSTRAINT_H

#include "../domain/domain.h"
#include "./uncertainty_constraint.h"
#include <unordered_map>
#include <vector>

class quadratic_uncertainty_constraint : public uncertainty_constraint {
public:
  quadratic_uncertainty_constraint(int constraint_id, int dimension, std::unique_ptr<domain> dom,
                                   matrix_d base_matrix_,
                                   std::vector<matrix_d> uncertain_matrices,
                                   double rhs = 0);

  ~quadratic_uncertainty_constraint() {}
  uncertainty_constraint::function_type get_function_type() const override {
    return uncertainty_constraint::QUADRATIC;
  }
  int dimension() const override { return domain_->dimension(); }
  int get_constraint_id() const override { return constraint_id_; }
  std::pair<double, vector_d> maximizer(const vector_d current) const override;
  vector_d gradient(const vector_d &current) const override;
  const domain *get_domain() const override { return domain_.get(); };
  double violation_amount(const vector_d &solution,
                          const vector_d &constraint_params) const override;
  const matrix_d &base_matrix() const { return base_matrix_; };
  const std::vector<matrix_d> &uncertain_matrices() const { return uncertain_matrices_; };

private:
  int constraint_id_;
  std::unique_ptr<domain> domain_;
  matrix_d base_matrix_;
  std::vector<matrix_d> uncertain_matrices_;
  double rhs_;

  matrix_d get_pairwise_uncertainty_quadratic(const vector_d &nominal_solution) const;
  vector_d get_linear_uncertainty_coefficients(const vector_d &nominal_solution) const;
  std::pair<double, vector_d> trs_subproblem_solution(const vector_d &nominal_solution) const;
};

#endif // ROBUST_CPP_QUADRATIC_UNCERTAINTY_CONSTRAINT_H
