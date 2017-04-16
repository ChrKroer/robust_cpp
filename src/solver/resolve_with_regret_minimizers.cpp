//
// Created by Christian Kroer on 4/14/17.
//

#include "./resolve_with_regret_minimizers.h"

resolve_with_regret_minimizers::resolve_with_regret_minimizers(
    const robust_program_dense *rp)
    : rp_(rp) {
  for (auto it = rp_->robust_constraints_begin();
       it != rp_->robust_constraints_end(); ++it) {
    // TODO: generate correct regret minimizer for the uncertainty set
    const uncertainty_set &unc_set = rp->get_uncertainty_set(*it);
  }
}
