//
// Created by Christian Kroer on 4/14/17.
//

#include "./resolve_with_regret_minimizers.h"
#include "./../logging.h"
#include "./../online_convex_optimization/online_mirror_descent.h"

resolve_with_regret_minimizers::resolve_with_regret_minimizers(
    const robust_program_dense *rp)
    : rp_(rp) {
  for (auto it = rp_->robust_constraints_begin();
       it != rp_->robust_constraints_end(); ++it) {
    int constraint_id = *it;
    const uncertainty_constraint &unc_set =
        rp->get_uncertainty_constraint(constraint_id);
    rms_[constraint_id] =
        std::make_unique<online_mirror_descent>(&unc_set.get_domain());
  }

  solution_ = vector_d(rp_->dimension());
  current_ = vector_d::Zero(rp_->dimension());
  for (int i = 0; i < rp_->dimension(); i++) {
    current_(i) = grb_model_->getVar(i).get(GRB_DoubleAttr_X);
  }
}

double resolve_with_regret_minimizers::optimize(int iterations_to_perform) {
  double max_gap = 0;
  double objective;
  int prev_iterations = iterations_;
  while (iterations_ < prev_iterations + iterations_to_perform) {
    for (auto it = rp_->robust_constraints_begin();
         it != rp_->robust_constraints_end(); ++it) {
      int constraint_id = *it;
      const uncertainty_constraint &unc_set =
          rp_->get_uncertainty_constraint(constraint_id);
      vector_d g = unc_set.gradient(current_);
      rms_[constraint_id]->receive_gradient(g);
      vector_d unc_set_current = rms_[constraint_id]->get_current_solution();
      update_uncertainty_constraint(constraint_id, unc_set_current);
    }
    grb_model_->optimize();
    int model_status = grb_model_->get(GRB_IntAttr_Status);
    if (model_status != GRB_OPTIMAL) {
      logger->info("Model not optimal, status: {}", model_status);
    }
    iterations_++;
    update_solution();
    objective = grb_model_->get(GRB_DoubleAttr_ObjVal);
    logger->info("objective on iteration {}: {}", iterations_, objective);
  }

  return objective;
}

void resolve_with_regret_minimizers::update_uncertainty_constraint(
    int constraint_id, const vector_d &coeff) {
  assert(coeff.size() == rp_->dimension());
  GRBConstr constr = grb_model_->getConstr(constraint_id);
  for (int i = 0; i < coeff.size(); i++) {
    GRBVar v = grb_model_->getVar(i);
    grb_model_->chgCoeff(constr, v, coeff(i));
  }
}

void resolve_with_regret_minimizers::update_solution() {
  for (int i = 0; i < rp_->dimension(); i++) {
    current_(i) = grb_model_->getVar(i).get(GRB_DoubleAttr_X);
    solution_(i) += current_(i);
  }
}
