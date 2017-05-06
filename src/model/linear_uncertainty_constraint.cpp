//
// Created by Christian Kroer on 4/12/17.
//

#include "./linear_uncertainty_constraint.h"
#include "./../logging.h"

linear_uncertainty_constraint::linear_uncertainty_constraint(
    int constraint_id, std::unique_ptr<domain> dom,
    std::vector<std::pair<int, double>> nominal_coeffs,
    std::vector<int> uncertainty_variable_ids, double rhs, char sense)
    : constraint_id_(constraint_id), domain_(std::move(dom)),
      nominal_coeffs_(nominal_coeffs),
      uncertainty_variable_ids_(uncertainty_variable_ids), rhs_(rhs),
      sense_(sense) {
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
  for (int i = 0; i < uncertainty_variable_ids_.size(); i++) {
    val += solution(uncertainty_variable_ids_[i]) * constraint_params(i);
  }
  for (auto &nominal_coeff : nominal_coeffs_) {
    if (var_id_to_uncertainty_id_.find(nominal_coeff.first) ==
        var_id_to_uncertainty_id_.end()) {
      val += solution(nominal_coeff.first) * nominal_coeff.second;
    }
  }

  return val - rhs_;
}

std::vector<std::pair<int, double>>
linear_uncertainty_constraint::get_full_coeffs(vector_d coeffs) const {
  std::vector<std::pair<int, double>> full_coeffs;
  for (auto &p : nominal_coeffs_) {
    if (var_id_to_uncertainty_id_.find(p.first) !=
        var_id_to_uncertainty_id_.end()) {
      // TODO: update for Nam's new format
      full_coeffs.push_back(std::make_pair(
          p.first, coeffs(var_id_to_uncertainty_id_.at(p.first))));
    } else {
      full_coeffs.push_back(p);
      full_coeffs.push_back(std::make_pair(p.first, p.second));
    }
  }
  return full_coeffs;
}
