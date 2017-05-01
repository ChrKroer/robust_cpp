//
// Created by Christian Kroer on 4/12/17.
//

#include "./linear_uncertainty_constraint.h"
#include "./../logging.h"

linear_uncertainty_constraint::linear_uncertainty_constraint(
    int dimension, std::unique_ptr<domain> dom, vector_d nominal_coeffs,
    std::vector<int> uncertainty_variable_ids, double rhs)
    : domain_(std::move(dom)), nominal_coeffs_(nominal_coeffs),
      uncertainty_variable_ids_(uncertainty_variable_ids), rhs_(rhs) {
  for (int i = 0; i < uncertainty_variable_ids.size(); i++) {
    var_id_to_uncertainty_id_[uncertainty_variable_ids_[i]] = i;
  }
}

std::pair<double, vector_d>
linear_uncertainty_constraint::maximizer(const vector_d current) const {
  vector_d g = gradient(current);
  return domain_->support(g);
};

vector_d
linear_uncertainty_constraint::gradient(const vector_d &current) const {
  vector_d g = vector_d(domain_->dimension());
  for (int i = 0; i < domain_->dimension(); i++) {
    g(i) = current(uncertainty_variable_ids_[i]);
  }
  return g;
}

double linear_uncertainty_constraint::violation_amount(
    const vector_d &solution, const vector_d &constraint_params) const {
  double val = 0;
  for (int i = 0; i < solution.size(); i++) {
    if (var_id_to_uncertainty_id_.find(i) == var_id_to_uncertainty_id_.end()) {
      int uncertainty_id = var_id_to_uncertainty_id_.at(i);
      val += solution(i) * constraint_params(uncertainty_id);
    } else {
      val += solution(i) * nominal_coeffs_(i);
    }
  }
  return val - rhs_;
}
