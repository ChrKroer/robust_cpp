//
// Created by Christian Kroer on 4/14/17.
//

#include "./../logging.h"
#include "./../online_convex_optimization/follow_the_perturbed_leader.h"
#include "./../online_convex_optimization/online_adam.h"
#include "./../online_convex_optimization/online_dual_averaging.h"
#include "./../online_convex_optimization/online_mirror_descent.h"
#include "./nominal_gurobi.h"
#include "./resolve_with_regret_minimizers.h"

resolve_with_regret_minimizers::resolve_with_regret_minimizers(
    const robust_program_dense *rp, const int when_to_average,
    const regret_minimizer rms_type, const double rms_stepsize_scalar,
    const double feasibility_tol)
    : rp_(rp),
      rms_type_(rms_type),
      rms_stepsize_scalar_(rms_stepsize_scalar),
      when_to_average_(when_to_average),
      feasibility_tol_(feasibility_tol) {
  for (auto it = rp_->robust_constraints_begin();
       it != rp_->robust_constraints_end(); ++it) {
    std::string constraint_name = *it;
    const uncertainty_constraint &unc =
        rp->get_uncertainty_constraint(constraint_name);
    if (rms_type_ == regret_minimizer::dual_averaging) {
      rms_[constraint_name] = std::make_unique<online_dual_averaging>(
          unc.get_domain(), rms_stepsize_scalar);
    } else if (rms_type_ == regret_minimizer::adam) {
      logger->debug("Running mirror descent with scalar {}",
                    rms_stepsize_scalar);
      rms_[constraint_name] =
          std::make_unique<online_adam>(unc.get_domain(), rms_stepsize_scalar);
    } else if (unc.get_function_type() == uncertainty_constraint::QUADRATIC &&
               rms_type_ == regret_minimizer::ftpl) {
      logger->debug("Running follow the perturbed leader with scalar {}",
                    rms_stepsize_scalar);
      const quadratic_uncertainty_constraint *unc_quad =
          dynamic_cast<const quadratic_uncertainty_constraint *>(&unc);

      rms_[constraint_name] = std::make_unique<follow_the_perturbed_leader>(
          unc.get_domain(), *unc_quad);
    } else {  // if (rms_type_ == regret_minimizer::project_gradient_descent) {
      logger->debug("Running mirror descent with scalar {}",
                    rms_stepsize_scalar);
      rms_[constraint_name] = std::make_unique<online_mirror_descent>(
          unc.get_domain(), rms_stepsize_scalar);
    }
  }
  solver_ = std::make_unique<nominal_gurobi>(rp->nominal_model_path());
  solver_->optimize();
  logger->debug("objective init: {}\n", solver_->get_objective());

  solution_ = vector_d::Zero(rp_->dimension());
  current_ = vector_d::Zero(rp_->dimension());
  for (int i = 0; i < rp_->dimension(); i++) {
    current_(i) = solver_->get_var_val(i);
    solution_(i) = current_(i);
  }
}

void resolve_with_regret_minimizers::solve_with_rms(
    const uncertainty_constraint &unc, const vector_d &x) {
  logger->debug("x: {}, norm: {}", eigen_to_string(x), x.norm());
  online_mirror_descent rms(unc.get_domain(), 0.1);
  vector_d unc_current = rms.get_current_solution();
  double violation_amount = unc.violation_amount(x, unc_current);
  logger->debug("rms violation: {}, norm: {}", violation_amount,
                unc_current.norm());
  unc.push_to_boundary(&unc_current, x);
  violation_amount = unc.violation_amount(current_solution(),
                                          unc_current / unc_current.norm());
  logger->debug("rms boundary violation: {}, norm: {}", violation_amount,
                unc_current.norm());
  for (int outer = 0; outer < 7; outer++) {
    unc_current = rms.get_current_solution();
    vector_d g = unc.gradient(x, unc_current);
    for (int inner = 0; inner < 1; inner++) {
      unc_current = rms.get_current_solution();
      g = unc.gradient(x, unc_current);
      rms.receive_gradient(-g);
    }
    logger->debug("g: {}, norm: {}", eigen_to_string(g), g.norm());
    logger->debug("unc: {}, norm: {}", eigen_to_string(unc_current),
                  unc_current.norm());

    violation_amount = unc.violation_amount(x, unc_current);
    logger->debug("rms violation: {}, norm: {}", violation_amount,
                  unc_current.norm());
    unc.push_to_boundary(&unc_current, x);
    violation_amount = unc.violation_amount(current_solution(),
                                            unc_current / unc_current.norm());
    logger->debug("rms boundary violation: {}, norm: {}", violation_amount,
                  unc_current.norm());
  }
  logger->debug("Unc solution: {}",
                eigen_to_string(rms.get_current_solution()));
  violation_amount = unc.violation_amount(x, rms.get_current_solution());
  logger->debug("rms violation: {}, norm: {}", violation_amount,
                rms.get_current_solution().norm());
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
      std::pair<double, vector_d> maximizer_current = unc.maximizer(current_);
      double violation_amount_current =
          unc.violation_amount(current_, maximizer_current.second);
      //   logger->debug("violation: {}, maximizer: {}, norm: {}",
      //   violation_amount,
      //                 eigen_to_string(maximizer.second),
      //                 maximizer.second.norm());
      //   logger->debug("(current) violation: {}, maximizer: {}, norm: {}",
      //                 violation_amount_current,
      //                 eigen_to_string(maximizer_current.second),
      //                 maximizer_current.second.norm());
      vector_d unc_current = rms_[constraint_name]->get_current_solution();
      if (unc.get_function_type() == uncertainty_constraint::QUADRATIC &&
          rms_type_ == regret_minimizer::ftpl) {
        auto rms_ftpl = dynamic_cast<follow_the_perturbed_leader *>(
            rms_[constraint_name].get());
        rms_ftpl->function_max(violation_amount);
        rms_ftpl->function_max(violation_amount_current);
        rms_[constraint_name]->receive_gradient(current_);
      } else {
        vector_d g = unc.gradient(current_, unc_current);
        rms_[constraint_name]->receive_gradient(-g);
      }

      logger->debug(
          "new rms: {} norm: {}",
          eigen_to_string(rms_[constraint_name]->get_current_solution()),
          rms_[constraint_name]->get_current_solution().norm());
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

  return objective_;
}

bool resolve_with_regret_minimizers::feasibility() {
  vector_d averaged_solution = current_solution();
  // logger->debug("Solution: {}", eigen_to_string(current));
  bool current_feasible = true;
  bool averaged_feasible = true;
  for (auto it = rp_->robust_constraints_begin();
       it != rp_->robust_constraints_end(); ++it) {
    std::string constraint_name = *it;
    const uncertainty_constraint &unc =
        rp_->get_uncertainty_constraint(constraint_name);

    std::pair<double, vector_d> maximizer = unc.maximizer(averaged_solution);
    double violation_amount =
        unc.violation_amount(averaged_solution, maximizer.second);
    logger->info("constraint {} violated by: {}", constraint_name,
                 violation_amount);
    if (violation_amount > feasibility_tol_) {
      averaged_feasible = false;
    }

    std::pair<double, vector_d> maximizer_current = unc.maximizer(current_);
    double violation_amount_current =
        unc.violation_amount(current_, maximizer_current.second);
    logger->info("(current) constraint {} violated by: {}", constraint_name,
                 violation_amount_current);
    if (violation_amount_current > feasibility_tol_) {
      current_feasible = false;
    }
    logger->debug("\n");
  }
  if (averaged_feasible) {
    return true;
  }
  if (current_feasible) {
    solution_ = current_;
    stopped_with_current_ = true;
    return true;
  }
  return false;
}

void resolve_with_regret_minimizers::resolve_and_update_solution() {
  solver_->optimize();
  solve_times_.push_back(solver_->get_runtime());
  logger->debug("Gurobi runtime: {}", solver_->get_runtime());
  // solver_->write_model("resolve_regret.lp");
  status_ = solver_->get_status();
  iterations_++;
  if (status_ != nominal_solver::OPTIMAL) {
    return;
  }
  if (iterations_ > when_to_average_) {
    objective_ *= (solution_normalizer_ - 1.0) / solution_normalizer_;
    objective_ += solver_->get_objective() / solution_normalizer_;
  } else {
    objective_ = solver_->get_objective();
  }
  for (int i = 0; i < rp_->dimension(); i++) {
    current_(i) = solver_->get_var_val(i);
  }
  if (iterations_ > when_to_average_) {
    solution_ *= (solution_normalizer_ - 1.0) / solution_normalizer_;
    solution_ += current_ / solution_normalizer_;
    solution_normalizer_++;
  } else {
    solution_ = current_;
  }
  // logger->debug("Current: {}", eigen_to_string(current_));
  // logger->debug("Solution: {}", eigen_to_string(current_solution()));
  logger->debug("Solution norm: {}", solution_.norm());
  logger->debug("Objective upper bound iteration {}: {}", iterations_,
                objective_);
  logger->debug("Objective on iteration {}: {}", iterations_,
                solver_->get_objective_for_solution(current_solution()));
}
