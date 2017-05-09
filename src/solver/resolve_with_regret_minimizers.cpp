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

double resolve_with_regret_minimizers::optimize() {
  do {
    logger->debug("Iteration {}", iterations_);

    // update each uncertain constraint
    for (auto it = rp_->robust_constraints_begin();
         it != rp_->robust_constraints_end(); ++it) {
      int constraint_id = *it;
      logger->debug("Constraint id: {}", constraint_id);
      const uncertainty_constraint &unc_set =
          rp_->get_uncertainty_constraint(constraint_id);
      vector_d g = unc_set.gradient(current_);
      logger->debug("Gradient: {}", eigen_to_string(g));
      // use -g because gradient represents maximization problem
      rms_[constraint_id]->receive_gradient(-g);
      vector_d unc_set_current = rms_[constraint_id]->get_current_solution();
      logger->debug(
          "current rms: {}",
          eigen_to_string(rms_[constraint_id]->get_current_solution()));
      solver_->update_constraint(constraint_id, unc_set_current, unc_set);
      logger->debug("\n");
    }
    // resolve
    resolve_and_update_solution();
    if (status_ != nominal_solver::OPTIMAL) {
      logger->debug("Infeasible");
      return 0;
    }
  } while (!feasibility());

  return objective_ / solution_normalizer_;
}

bool resolve_with_regret_minimizers::feasibility() {
  for (auto it = rp_->robust_constraints_begin();
       it != rp_->robust_constraints_end(); ++it) {
    int constraint_id = *it;
    logger->debug("Constraint id: {}", constraint_id);
    const uncertainty_constraint &unc_set =
        rp_->get_uncertainty_constraint(constraint_id);
    std::pair<double, vector_d> maximizer =
        unc_set.maximizer(current_solution());
    logger->debug("max val: {}", maximizer.first);
    double violation_amount =
        unc_set.violation_amount(current_solution(), maximizer.second);
    logger->debug("violation amount: {}", violation_amount);
    logger->debug("maximizer: {}", eigen_to_string(maximizer.second));
    if (violation_amount > abs_tol_ &&
        violation_amount > rel_tol_ * maximizer.second.norm()) {
      // violation_amount > rel_rol_ *.norm()) {
      logger->info("constraint {} violated by: {}", constraint_id,
                   violation_amount);
      return false;
    }
    logger->debug("\n");
  }
  return true;
}

void resolve_with_regret_minimizers::resolve_and_update_solution() {
  solver_->optimize();
  solver_->write_model("resolve_regret_final.lp");
  status_ = solver_->get_status();
  iterations_++;
  if (status_ != nominal_solver::OPTIMAL) {
    return;
  }
  if (iterations_ > 10) {
    objective_ += solver_->get_objective();
    solution_normalizer_++;
  } else {
    objective_ = solver_->get_objective();
  }
  logger->debug("objective on iteration {}: {}\n", iterations_,
                objective_ / solution_normalizer_);
  for (int i = 0; i < rp_->dimension(); i++) {
    current_(i) = solver_->get_var_val(i);
    // heuristic; don't average until a few iterations in
    if (iterations_ > 10) {
      solution_(i) += current_(i);
    } else {
      solution_(i) = current_(i);
    }
  }
}
