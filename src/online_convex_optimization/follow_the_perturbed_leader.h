//
// Created by Christian Kroer on 3/30/17.
//

#ifndef ROBUST_CPP_FOLLOW_THE_PERTURBED_LEADER_H
#define ROBUST_CPP_FOLLOW_THE_PERTURBED_LEADER_H

#include <Eigen/Core>
#include <algorithm>
#include <tuple>
#include "../basic_types.h"
#include "./../model/quadratic_uncertainty_constraint.h"
#include "./online_gradient_method.h"

class follow_the_perturbed_leader : public online_gradient_method {
 public:
  explicit follow_the_perturbed_leader(
      const domain *dom, const double stepsize_scalar,
      const quadratic_uncertainty_constraint &unc_constraint);

  void receive_gradient(vector_d nominal_solution) override;
  const vector_d &get_current_solution() override { return iterate_; }
  void set_solution(vector_d solution) override { iterate_ = solution; }
  const domain &get_domain() override { return *domain_; }

  /* @brief sets the maximum seen value of the functions we are
   * regret-minimizing over.
   * Must be called at least once before first call to receive_gradient.
   *
   */
  void function_max(double new_val) {
    f_max_ = std::max(std::abs(new_val), f_max_);
  }

 private:
  void init();

  int num_iterations_ = 0;
  vector_d iterate_;
  double diameter_;
  double f_max_ = 0;
  double gradient_max_ = 0;
  const domain *domain_;
  const double stepsize_scalar_;
  const quadratic_uncertainty_constraint &uncertainty_constraint_;

  matrix_d averaged_matrix_;
  vector_d averaged_vector_;

  matrix_d noisy_matrix() const;
  vector_d noisy_vector() const;
  void update_gradient_max(const matrix_d &Y, const vector_d &lin);
};

#endif  // ROBUST_CPP_FOLLOW_THE_PERTURBED_LEADER_H
