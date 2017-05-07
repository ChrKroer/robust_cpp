//
// Created by Christian Kroer on 4/04/17.
//

#include "pessimization_solver.h"
#include "./../logging.h"
#include "./../model/linear_uncertainty_constraint.h"
#include "./nominal_gurobi.h"
#include "gurobi_c++.h"

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
    logger->debug("\n\nIteration {}", num_iterations_);
    violated = false;
    vector_d current = current_solution();
    for (auto it = rp_->robust_constraints_begin();
         it != rp_->robust_constraints_end(); ++it) {
      int constraint_id = *it;
      logger->debug("\n\nConstraint id: {}", constraint_id);
      const uncertainty_constraint &unc =
          rp_->get_uncertainty_constraint(constraint_id);
      std::pair<double, vector_d> maximizer = unc.maximizer(current);
      logger->debug("max val: {}", maximizer.first);
      logger->debug("maximizer: {}", eigen_to_string(maximizer.second));
      if (unc.violation_amount(current, maximizer.second) > tolerance_) {
        violated = true;
        solver_->add_constraint(maximizer.second, unc);
      }
    }
    solver_->optimize();
    num_iterations_++;
    status_ = solver_->get_status();
    if (status_ == nominal_solver::OPTIMAL) {
      objective = solver_->get_objective();
    } else {
      logger->debug("Infeasible");
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
