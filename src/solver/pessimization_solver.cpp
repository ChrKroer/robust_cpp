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
      const uncertainty_constraint &set =
          rp_->get_uncertainty_constraint(constraint_id);
      std::pair<double, vector_d> maximizer = set.maximizer(current);
      logger->debug("max val: {}", maximizer.first);
      logger->debug("maximizer: {}", eigen_to_string(maximizer.second));
      if (set.violation_amount(current, maximizer.second) > tolerance_) {
        violated = true;
        add_uncertainty_constraint(constraint_id, maximizer.second);
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

void pessimization_solver::add_uncertainty_constraint(int constraint_id,
                                                      vector_d coeffs) {
  if (rp_->get_uncertainty_constraint(constraint_id).get_function_type() ==
      uncertainty_constraint::LINEAR) {
    const linear_uncertainty_constraint &set =
        dynamic_cast<const linear_uncertainty_constraint &>(
            rp_->get_uncertainty_constraint(constraint_id));
    std::vector<std::pair<int, double>> full_coeffs =
        set.get_full_coeffs(coeffs);
    double rhs = set.get_rhs();
    solver_->add_linear_constraint(full_coeffs, rhs);
  } else {
    logger->error("constraint type not yet supported.");
    std::exit(1);
  }
}

vector_d pessimization_solver::current_solution() {
  vector_d strategy(rp_->dimension());
  for (int i = 0; i < rp_->dimension(); i++) {
    strategy(i) = solver_->get_var_val(i);
  }
  return strategy;
}
