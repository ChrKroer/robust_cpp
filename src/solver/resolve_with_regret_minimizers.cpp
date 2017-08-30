//
// Created by Christian Kroer on 4/14/17.
//

#include "./../logging.h"
#include "./../online_convex_optimization/online_dual_averaging.h"
#include "./../online_convex_optimization/online_mirror_descent.h"
#include "./nominal_gurobi.h"
#include "./resolve_with_regret_minimizers.h"

resolve_with_regret_minimizers::resolve_with_regret_minimizers(
    const robust_program_dense *rp, int when_to_average, bool dual_average,
    double rm_stepsize_scalar)
    : rp_(rp), when_to_average_(when_to_average) {
  for (auto it = rp_->robust_constraints_begin();
       it != rp_->robust_constraints_end(); ++it) {
    std::string constraint_name = *it;
    const uncertainty_constraint &unc =
        rp->get_uncertainty_constraint(constraint_name);
    if (dual_average) {
      rms_[constraint_name] = std::make_unique<online_dual_averaging>(
          unc.get_domain(), rm_stepsize_scalar);
    } else {
      logger->debug("Running mirror descent with scalar {}",
                    rm_stepsize_scalar);
      rms_[constraint_name] = std::make_unique<online_mirror_descent>(
          unc.get_domain(), rm_stepsize_scalar);
    }
  }
  solver_ = std::make_unique<nominal_gurobi>(rp->nominal_model_path());
  solver_->optimize();
  logger->debug("objective init: {}\n", solver_->get_objective());

  solution_ = vector_d::Zero(rp_->dimension());
  objective_ = solver_->get_objective();
  current_ = vector_d::Zero(rp_->dimension());
  for (int i = 0; i < rp_->dimension(); i++) {
    current_(i) = solver_->get_var_val(i);
    solution_(i) = current_(i);
  }
}

double resolve_with_regret_minimizers::optimize() {
  do {
    logger->debug("\n\nIteration {}", iterations_);

    // update each uncertain constraint
    for (auto it = rp_->robust_constraints_begin();
         it != rp_->robust_constraints_end(); ++it) {
      std::string constraint_name = *it;
      logger->debug("Constraint Name: {}", constraint_name);
      const uncertainty_constraint &unc =
          rp_->get_uncertainty_constraint(constraint_name);
      std::pair<double, vector_d> maximizer = unc.maximizer(current_solution());
      double violation_amount =
          unc.violation_amount(current_solution(), maximizer.second);
      logger->debug("violation amount before update: {}", violation_amount);
      vector_d unc_current = rms_[constraint_name]->get_current_solution();
      vector_d g = unc.gradient(current_, unc_current);
      //   logger->debug("Gradient: {}", eigen_to_string(g));
      // use -g because gradient represents maximization problem
      rms_[constraint_name]->receive_gradient(-g);
      //   logger->debug(
      //       "new rms: {} norm: {}",
      //       eigen_to_string(rms_[constraint_name]->get_current_solution()),
      //       rms_[constraint_name]->get_current_solution().norm());
      vector_d unc_new = rms_[constraint_name]->get_current_solution();
      solver_->update_constraint(unc_new, unc);
    }
    // resolve
    resolve_and_update_solution();
    if (status_ != nominal_solver::OPTIMAL) {
      logger->debug("Nonoptimal gurobi status: {}",
                    solver_->get_string_status());
      solver_->write_model("resolve_regret.lp");
      return 0;
    }
  } while (!feasibility());

  return objective_ / solution_normalizer_;
}

bool resolve_with_regret_minimizers::feasibility() {
  vector_d current = current_solution();
  // logger->debug("Solution: {}", eigen_to_string(current));
  for (auto it = rp_->robust_constraints_begin();
       it != rp_->robust_constraints_end(); ++it) {
    std::string constraint_name = *it;
    logger->debug("Constraint Name: {}", constraint_name);
    const uncertainty_constraint &unc =
        rp_->get_uncertainty_constraint(constraint_name);
    std::pair<double, vector_d> maximizer = unc.maximizer(current);
    double violation_amount = unc.violation_amount(current, maximizer.second);
    // logger->debug("maximizer: {}", eigen_to_string(maximizer.second));
    if (violation_amount > abs_tol_ &&
        violation_amount > rel_tol_ * maximizer.second.norm()) {
      // violation_amount > rel_rol_ *.norm()) {
      logger->debug("constraint {} violated by: {}", constraint_name,
                    violation_amount);
      return false;
    }
    logger->debug("\n");
  }
  return true;
}

void resolve_with_regret_minimizers::resolve_and_update_solution() {
  solver_->optimize();
  solver_->write_model("resolve_regret.lp");
  status_ = solver_->get_status();
  iterations_++;
  if (status_ != nominal_solver::OPTIMAL) {
    return;
  }
  if (iterations_ > when_to_average_) {
    objective_ += solver_->get_objective();
    solution_normalizer_++;
  } else {
    objective_ = solver_->get_objective();
  }
  for (int i = 0; i < rp_->dimension(); i++) {
    current_(i) = solver_->get_var_val(i);
    // heuristic; don't average until a few iterations in
  }
  if (iterations_ > when_to_average_) {
    solution_ += current_;
  } else {
    solution_ = current_;
  }
  // for (auto it = rp_->robust_constraints_begin();
  //      it != rp_->robust_constraints_end(); ++it) {
  //   std::string constraint_name = *it;
  //   const uncertainty_constraint &unc =
  //       rp_->get_uncertainty_constraint(constraint_name);
  //   vector_d unc_new = rms_[constraint_name]->get_current_solution();
  //   std::pair<double, vector_d> maximizer =
  //   unc.maximizer(current_solution());
  //   logger->debug("Violation amount after optimize: {}", maximizer.first);
  //   logger->debug("Violation amount rms coeffs: {}",
  //                 unc.violation_amount(current_solution(), unc_new));
  // }
  // vector_d current = current_solution();
  // logger->debug("Solution: {}", eigen_to_string(current));
  logger->debug("Objective upper bound iteration {}: {}", iterations_,
                objective_ / solution_normalizer_);
  logger->debug("Objective on iteration {}: {}", iterations_,
                solver_->get_objective_for_solution(current_solution()));
}
