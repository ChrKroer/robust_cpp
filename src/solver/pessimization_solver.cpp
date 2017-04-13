//
// Created by Christian Kroer on 4/04/17.
//

#include "pessimization_solver.h"
#include "./../logging.h"
#include "gurobi_c++.h"

pessimization_solver::pessimization_solver(const robust_program_dense *rp)
    : rp_(rp), grb_env_() {
  grb_env_.set(GRB_IntParam_Threads, 1);
  grb_env_.set(GRB_IntParam_OutputFlag, 0);
  grb_model_ = std::make_unique<GRBModel>(grb_env_, rp->nominal_model_path());
}

double pessimization_solver::optimize() {
  grb_model_->optimize();
  int status = grb_model_->get(GRB_IntAttr_Status);
  double objective;
  if (status == GRB_INFEASIBLE) {
    logger->warn("### Infeasible nominal");
    return -1;
  } else {
    objective = grb_model_->get(GRB_DoubleAttr_ObjVal);
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
      const uncertainty_set &set = rp_->get_uncertainty_set(constraint_id);
      std::pair<double, vector_d> maximizer = set.maximizer(current);
      if (maximizer.first > 0) {
        violated = true;
        add_uncertainty_constraint(constraint_id, maximizer.second);
      }
    }
    grb_model_->optimize();
    objective = grb_model_->get(GRB_DoubleAttr_ObjVal);
    logger->info("objective on iteration {}: {}", num_resolves, objective);
  }

  return objective;
}

void pessimization_solver::add_uncertainty_constraint(int constraint_id,
                                                      vector_d coeff) {
  if (rp_->get_constraint_type(constraint_id) == robust_program::linear) {
    assert(coeff.size() == rp_->dimension() + 1);
    GRBLinExpr newConstr = 0;
    for (int i = 0; i < rp_->dimension(); i++) {
      newConstr += grb_model_->getVar(i) * coeff(i);
    }
    grb_model_->addConstr(newConstr <= coeff(coeff.size() - 1));
  } else {
    logger->error("constraint type not yet supported.");
    std::exit(1);
  }
}

vector_d pessimization_solver::current_strategy() {
  vector_d strategy(rp_->dimension());
  for (int i = 0; i < rp_->dimension(); i++) {
    strategy(i) = grb_model_->getVar(i).get(GRB_DoubleAttr_X);
  }
  return strategy;
}
