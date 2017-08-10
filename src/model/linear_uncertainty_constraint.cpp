//
// Created by Christian Kroer on 4/12/17.
//

#include "./../logging.h"
#include "./linear_uncertainty_constraint.h"

linear_uncertainty_constraint::linear_uncertainty_constraint(
    std::string constraint_name, std::unique_ptr<domain> dom,
    sparse_vector_d nominal_coeffs, vector_d weights,
    std::vector<int> uncertainty_variable_ids, double rhs, char sense,
    std::vector<double> certain_variable_coefficient,
    std::vector<int> certain_variable_index,
    std::vector<std::string> certain_variable_name)
    : domain_(std::move(dom)),
      nominal_coeffs_(nominal_coeffs),
      weights_(weights),
      uncertainty_variable_ids_(uncertainty_variable_ids),
      rhs_(rhs),
      sense_(sense) {
  for (int i = 0; i < uncertainty_variable_ids.size(); i++) {
    var_id_to_uncertainty_id_[uncertainty_variable_ids_[i]] = i;
  }
  uncertain_nominal_coeffs_ = vector_d(weights_.size());
  for (sparse_vector_d::InnerIterator it(nominal_coeffs_); it; ++it) {
    int id = it.index();
    if (var_id_to_uncertainty_id_.find(id) != var_id_to_uncertainty_id_.end()) {
      uncertain_nominal_coeffs_(var_id_to_uncertainty_id_[id]) = it.value();
    }
  }
  name_ = constraint_name;
  certain_variable_coefficient_ = certain_variable_coefficient;
  certain_variable_index_ = certain_variable_index;
  certain_variable_name_ = certain_variable_name;
}

std::pair<double, vector_d> linear_uncertainty_constraint::maximizer(
    const vector_d current) const {
  vector_d g = gradient(current);  // since it's a linear function, the gradient
                                   // is also the coefficients
  std::pair<double, vector_d> p = domain_->support(g);
  return std::make_pair(violation_amount(current, p.second), p.second);
}

vector_d linear_uncertainty_constraint::gradient(
    const vector_d &current) const {
  vector_d g = vector_d(domain_->dimension());
  for (int i = 0; i < domain_->dimension(); i++) {
    g(i) = weights_[i] * current(uncertainty_variable_ids_[i]);
  }
  return g;
}

vector_d linear_uncertainty_constraint::gradient(
    const vector_d &solution, const vector_d &unc_vec) const {
  return gradient(solution);
}

double linear_uncertainty_constraint::violation_amount(
    const vector_d &solution, const vector_d &constraint_params) const {
  double val = 0;
  for (sparse_vector_d::InnerIterator it(nominal_coeffs_); it; ++it) {
    int id = it.index();
    double coeff = it.value();
    if (var_id_to_uncertainty_id_.find(id) != var_id_to_uncertainty_id_.end()) {
      int uncertainty_id = var_id_to_uncertainty_id_.at(id);
      coeff += constraint_params(uncertainty_id) * weights_[uncertainty_id];
    }
    val += coeff * solution(id);
  }
  for (int i = 0; i < certain_variable_coefficient_.size(); i++) {
    val +=
        certain_variable_coefficient_[i] * solution(certain_variable_index_[i]);
  }

  return val - rhs_;
}

sparse_vector_d linear_uncertainty_constraint::get_full_coeffs(
    vector_d u_coeffs) const {
  sparse_vector_d full_coeffs(nominal_coeffs_.size());
  full_coeffs.reserve(nominal_coeffs_.nonZeros());
  for (sparse_vector_d::InnerIterator it(nominal_coeffs_); it; ++it) {
    int id = it.index();
    double val = it.value();
    if (var_id_to_uncertainty_id_.find(id) != var_id_to_uncertainty_id_.end()) {
      int uncertainty_id = var_id_to_uncertainty_id_.at(id);
      val += u_coeffs(uncertainty_id) * weights_[uncertainty_id];
    }
    full_coeffs.insert(id) = val;
  }
  return full_coeffs;
}

double linear_uncertainty_constraint::get_coeff(
    int uncertainty_id, double uncertainty_coeff) const {
  return uncertainty_coeff * weights_[uncertainty_id] +
         uncertain_nominal_coeffs_[uncertainty_id];
}
