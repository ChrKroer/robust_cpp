//
// Created by Christian Kroer on 4/04/17.
//

#include "pessimization_solver.h"
#include "./../logging.h"
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
    logger->info("nominal objective: {}", objective);
  }

  bool violated = true;
  int num_resolves = 0;
  while (violated) {
    violated = false;
    vector_d current = current_strategy();
    for (auto it = rp_->robust_constraints_begin();
         it != rp_->robust_constraints_end(); ++it) {
      int constraint_id = *it;
      const uncertainty_constraint &set =
          rp_->get_uncertainty_constraint(constraint_id);
      std::pair<double, vector_d> maximizer = set.maximizer(current);
      double rhs = solver_->get_rhs(constraint_id);
      if (maximizer.first - tolerance > rhs) {
        violated = true;
        add_uncertainty_constraint(constraint_id, maximizer.second);
      }
    }
    solver_->optimize();
    num_resolves++;
    objective = solver_->get_objective();
    logger->info("objective on iteration {}: {}", num_resolves, objective);
  }

  return objective;
}

void pessimization_solver::add_uncertainty_constraint(int constraint_id,
                                                      vector_d coeffs) {
  if (rp_->get_uncertainty_constraint(constraint_id).get_function_type() ==
      uncertainty_constraint::LINEAR) {
    const uncertainty_constraint &set =
        rp_->get_uncertainty_constraint(constraint_id);
    const std::vector<int> &var_ids = set.uncertainty_variable_ids();
    double rhs = solver_->get_rhs(constraint_id);
    solver_->add_linear_constraint(var_ids, coeffs, rhs);
  } else {
    logger->error("constraint type not yet supported.");
    std::exit(1);
  }
}

vector_d pessimization_solver::current_strategy() {
  vector_d strategy(rp_->dimension());
  for (int i = 0; i < rp_->dimension(); i++) {
    strategy(i) = solver_->get_var_val(i);
  }
  return strategy;
}
