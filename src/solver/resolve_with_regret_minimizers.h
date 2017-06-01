//
// Created by Christian Kroer on 4/14/17.
//

#ifndef ROBUST_CPP_RESOLVE_WITH_REGRET_MINIMIZERS_H
#define ROBUST_CPP_RESOLVE_WITH_REGRET_MINIMIZERS_H

#include "./../basic_types.h"
#include "./../model/robust_program_dense.h"
#include "./../online_convex_optimization/online_gradient_method.h"
#include "./nominal_gurobi.h"
#include "./robust_solver.h"
#include <unordered_map>

class resolve_with_regret_minimizers : public robust_solver {
public:
  explicit resolve_with_regret_minimizers(const robust_program_dense *rp);

  vector_d current_solution() override { return solution_ / solution_normalizer_; }
  double optimize() override;
  int num_iterations() override { return iterations_; }
  nominal_solver::status get_status() override { return status_; }

private:
  const robust_program *rp_;
  // indexed by the constraint_id that the rms belongs to
  std::unordered_map<int, std::unique_ptr<online_gradient_method>> rms_;

  std::unique_ptr<nominal_gurobi> solver_;
  nominal_solver::status status_;
  double abs_tol_ = 1e-5;
  double rel_tol_ = 1e-3;
  int iterations_ = 0;
  vector_d solution_;
  int solution_normalizer_ = 1;
  vector_d current_;
  double objective_ = 0;

  void resolve_and_update_solution();
  bool feasibility();
};

#endif // ROBUST_CPP_RESOLVE_WITH_REGRET_MINIMIZERS_H
