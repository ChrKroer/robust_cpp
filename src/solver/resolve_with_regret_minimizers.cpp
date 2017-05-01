//
// Created by Christian Kroer on 4/14/17.
//

#include "./resolve_with_regret_minimizers.h"
#include "./../logging.h"
#include "./../online_convex_optimization/online_mirror_descent.h"
#include "./nominal_gurobi.h"

resolve_with_regret_minimizers::resolve_with_regret_minimizers(
    const robust_program_dense *rp)
    : rp_(rp) {
  for (auto it = rp_->robust_constraints_begin();
       it != rp_->robust_constraints_end(); ++it) {
    int constraint_id = *it;
    const uncertainty_constraint &unc_set =
        rp->get_uncertainty_constraint(constraint_id);
    rms_[constraint_id] =
        std::make_unique<online_mirror_descent>(unc_set.get_domain());
  }
  solver_ = std::make_unique<nominal_gurobi>(rp->nominal_model_path());
  solver_->optimize();

  solution_ = vector_d(rp_->dimension());
  current_ = vector_d::Zero(rp_->dimension());
  for (int i = 0; i < rp_->dimension(); i++) {
    current_(i) = solver_->get_var_val(i);
  }
}

double resolve_with_regret_minimizers::optimize(int iterations_to_perform) {
  double max_gap = 0;
  int prev_iterations = iterations_;
  while (iterations_ < prev_iterations + iterations_to_perform) {
    for (auto it = rp_->robust_constraints_begin();
         it != rp_->robust_constraints_end(); ++it) {
      int constraint_id = *it;
      const uncertainty_constraint &unc_set =
          rp_->get_uncertainty_constraint(constraint_id);
      vector_d g = unc_set.gradient(current_);
      // use -g because gradient represents maximization problem
      rms_[constraint_id]->receive_gradient(-g);
      vector_d unc_set_current = rms_[constraint_id]->get_current_solution();
      update_uncertainty_constraint(constraint_id, unc_set_current);
    }
    solver_->optimize();
    solver_->write_model("resolve_regret_final.lp");
    nominal_solver::status model_status = solver_->get_status();
    if (model_status != nominal_solver::OPTIMAL) {
      logger->info("Model not optimal, status: {}", model_status);
    }
    iterations_++;
    update_solution();
    objective_ += solver_->get_objective();
    logger->info("objective on iteration {}: {}", iterations_,
                 objective_ / iterations_);
  }

  return objective_ / iterations_;
}

void resolve_with_regret_minimizers::update_uncertainty_constraint(
    int constraint_id, const vector_d &coeffs) {
  const uncertainty_constraint &unc_set =
      rp_->get_uncertainty_constraint(constraint_id);
  const std::vector<int> &var_ids = unc_set.uncertainty_variable_ids();
  solver_->update_constraint(constraint_id, var_ids, coeffs);
}

void resolve_with_regret_minimizers::update_solution() {
  for (int i = 0; i < rp_->dimension(); i++) {
    current_(i) = solver_->get_var_val(i);
    solution_(i) += current_(i);
  }
}
