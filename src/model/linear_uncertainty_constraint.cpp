//
// Created by Christian Kroer on 4/12/17.
//

#include "./linear_uncertainty_constraint.h"
#include "./../logging.h"

linear_uncertainty_constraint::linear_uncertainty_constraint(
    int constraint_id, std::unique_ptr<domain> dom,
    sparse_vector_d nominal_coeffs, vector_d weights,
    std::vector<int> uncertainty_variable_ids, double rhs, char sense)
    : constraint_id_(constraint_id), domain_(std::move(dom)),
      nominal_coeffs_(nominal_coeffs), weights_(weights),
      uncertainty_variable_ids_(uncertainty_variable_ids), rhs_(rhs),
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
}

std::pair<double, vector_d>
linear_uncertainty_constraint::maximizer(const vector_d current) const {
  vector_d g = gradient(current);
  for (int i = 0; i < g.size(); i++) {
    g *= weights_[i];
  }
  std::pair<double, vector_d> p = domain_->support(g);
  for (int i = 0; i < g.size(); i++) {
    p.second *= weights_[i];
  }
  return p;
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
  for (sparse_vector_d::InnerIterator it(nominal_coeffs_); it; ++it) {
    int id = it.index();
    double coeff = it.value();
    if (var_id_to_uncertainty_id_.find(id) != var_id_to_uncertainty_id_.end()) {
      coeff += constraint_params(var_id_to_uncertainty_id_.at(id));
    }
    val += coeff * solution(id);
  }

  return val - rhs_;
}

sparse_vector_d
linear_uncertainty_constraint::get_full_coeffs(vector_d u_coeffs) const {
  sparse_vector_d full_coeffs(nominal_coeffs_.size());
  full_coeffs.reserve(nominal_coeffs_.nonZeros());
  for (sparse_vector_d::InnerIterator it(nominal_coeffs_); it; ++it) {
    int id = it.index();
    double val = it.value();
    if (var_id_to_uncertainty_id_.find(id) != var_id_to_uncertainty_id_.end()) {
      val += u_coeffs(var_id_to_uncertainty_id_.at(id));
    }
    full_coeffs.insert(id) = val;
  }
  return full_coeffs;
}
