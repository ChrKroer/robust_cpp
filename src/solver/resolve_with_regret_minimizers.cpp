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
}

double resolve_with_regret_minimizers::optimize(int iterations_to_perform) {
  double max_gap = 0;
  double objective;
  int iterations = 0;
  while (iterations < iterations_to_perform) {
    for (auto it = rp_->robust_constraints_begin();
         it != rp_->robust_constraints_end(); ++it) {
      int constraint_id = *it;
      vector_d g; // TODO: compute gradient
      rms_[constraint_id]->receive_gradient(g);
      vector_d current = rms_[constraint_id]->get_current_solution();
      update_uncertainty_constraint(constraint_id, current);
    }
    grb_model_->optimize();
    iterations++;
    objective = grb_model_->get(GRB_DoubleAttr_ObjVal);
    logger->info("objective on iteration {}: {}", iterations, objective);
  }

  return objective;
}

void resolve_with_regret_minimizers::update_uncertainty_constraint(
    int constraint_id, vector_d coeff) {
  // TODO: implement this
}
