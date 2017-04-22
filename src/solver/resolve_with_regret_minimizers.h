//
// Created by Christian Kroer on 4/14/17.
//

#ifndef ROBUST_CPP_RESOLVE_WITH_REGRET_MINIMIZERS_H
#define ROBUST_CPP_RESOLVE_WITH_REGRET_MINIMIZERS_H

#include "../basic_types.h"
#include "./../model/robust_program_dense.h"
#include "./../online_convex_optimization/online_gradient_method.h"
#include "gurobi_c++.h"
#include <unordered_map>

class resolve_with_regret_minimizers {
public:
  explicit resolve_with_regret_minimizers(const robust_program_dense *rp);
  ~resolve_with_regret_minimizers() {}

  vector_d current_solution() { return solution_ / iterations_; }
  double optimize(int iterations_to_perform);

private:
  const robust_program *rp_;
  // indexed by the constraint_id that the rms belongs to
  std::unordered_map<int, std::unique_ptr<online_gradient_method>> rms_;

  GRBEnv grb_env_;
  std::unique_ptr<GRBModel> grb_model_;
  double tolerance_ = 1e-6;
  int iterations_ = 0;
  vector_d solution_;
  vector_d current_;

  void update_uncertainty_constraint(int constraint_id, const vector_d &coeff);
  void update_solution();
};

#endif // ROBUST_CPP_RESOLVE_WITH_REGRET_MINIMIZERS_H
