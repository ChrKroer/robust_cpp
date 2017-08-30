//
// Created by Christian Kroer on 4/04/17.
//

#include <string>
#include <utility>
#include "./../logging.h"
#include "./../model/linear_uncertainty_constraint.h"
#include "./nominal_gurobi.h"
#include "gurobi_c++.h"
#include "pessimization_solver.h"

pessimization_solver::pessimization_solver(const robust_program_dense *rp)
    : rp_(rp) {
  solver_ = std::make_unique<nominal_gurobi>(rp->nominal_model_path());
}

double pessimization_solver::optimize() {
  solver_->optimize();
  nominal_solver::status status = solver_->get_status();
  double objective;
  if (status != nominal_solver::OPTIMAL) {
    logger->warn("### Non-optimal nominal");
    return -1;
  } else {
    objective = solver_->get_objective();
    logger->debug("nominal objective: {}", objective);
  }

  bool violated = true;
  num_iterations_ = 0;
  while (violated) {
    logger->debug("");
    logger->debug("Iteration {}", num_iterations_);
    vector_d current = current_solution();
    logger->debug("Solution: {}", eigen_to_string(current));
    violated = false;
    for (auto it = rp_->robust_constraints_begin();
         it != rp_->robust_constraints_end(); ++it) {
      std::string constraint_name = *it;
      logger->debug("Constraint Name: {}", constraint_name);
      const uncertainty_constraint &unc =
          rp_->get_uncertainty_constraint(constraint_name);
      std::pair<double, vector_d> maximizer = unc.maximizer(current_solution());
      //   logger->debug("maximizer: {} norm: {}",
      //   eigen_to_string(maximizer.second),
      //                 maximizer.second.norm());
      logger->debug("norm: {}", maximizer.second.norm());

      double violation_amount = unc.violation_amount(current, maximizer.second);
      logger->debug("Violation amount: {}", violation_amount);
      //   for (const vector_d coeffs : coeffs_added_) {
      //     double v = unc.violation_amount(current, coeffs);
      //     logger->debug("Violation amount past: {}, norm: {}", v,
      //     coeffs.norm());
      //   }
      if (violation_amount > tolerance_) {
        // check what maximizer
        solver_->add_constraint(maximizer.second, unc);
        coeffs_added_.push_back(maximizer.second);
        violated = true;
        logger->debug("violated");
      }
    }
    solver_->write_model("pessimization.lp");
    solver_->optimize();
    num_iterations_++;
    status_ = solver_->get_status();
    if (status_ == nominal_solver::OPTIMAL) {
      objective = solver_->get_objective();
    } else {
      logger->debug("Nonoptimal gurobi status: {}",
                    solver_->get_string_status());
      solver_->write_model("pessimization_final.lp");
      return 0;
    }
    logger->debug("objective on iteration {}: {}", num_iterations_, objective);
  }

  return objective;
}

vector_d pessimization_solver::current_solution() {
  vector_d strategy(rp_->dimension());
  for (int i = 0; i < rp_->dimension(); i++) {
    strategy(i) = solver_->get_var_val(i);
  }
  return strategy;
}
