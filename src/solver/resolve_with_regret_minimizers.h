//
// Created by Christian Kroer on 4/14/17.
//

#ifndef ROBUST_CPP_RESOLVE_WITH_REGRET_MINIMIZERS_H
#define ROBUST_CPP_RESOLVE_WITH_REGRET_MINIMIZERS_H

#include <string>
#include <unordered_map>
#include <vector>
#include "./../basic_types.h"
#include "./../model/robust_program_dense.h"
#include "./../online_convex_optimization/online_gradient_method.h"
#include "./nominal_gurobi.h"
#include "./robust_solver.h"

class resolve_with_regret_minimizers : public robust_solver {
 public:
  enum class regret_minimizer {
    project_gradient_descent,
    dual_averaging,
    adam,
    ftpl
  };
  resolve_with_regret_minimizers(const robust_program_dense *rp,
                                 const int when_to_average = 2,
                                 const regret_minimizer rms_type =
                                     regret_minimizer::project_gradient_descent,
                                 const double rms_stepsize_scalar = 1.0,
                                 const double feasibility_tol = 1e-3);

  vector_d current_solution() override { return solution_; }
  double optimize() override;
  int num_iterations() override { return iterations_; }
  nominal_solver::status get_status() override { return status_; }
  const std::vector<double> &solve_times() const override {
    return solve_times_;
  }
  bool stopped_with_current() const { return stopped_with_current_; }
  const std::vector<double> &max_violations() const override {
    return max_violations_per_iter_;
  }

 private:
  const robust_program *rp_;
  // indexed by the constraint_id that the rms belongs to
  std::unordered_map<std::string, std::unique_ptr<online_gradient_method>> rms_;
  std::vector<double> solve_times_;
  const regret_minimizer rms_type_;
  const double rms_stepsize_scalar_;

  std::vector<double> max_violations_per_iter_;

  std::unique_ptr<nominal_gurobi> solver_;
  nominal_solver::status status_;
  const int when_to_average_ = 2;
  const double feasibility_tol_;
  const double rel_tol_ = 1e-2;
  int iterations_ = 0;
  vector_d solution_;
  int solution_normalizer_ = 1;
  vector_d current_;
  double objective_ = 0;
  bool stopped_with_current_ = false;

  void solve_with_rms(const uncertainty_constraint &unc, const vector_d &x);
  void resolve_and_update_solution();
  bool feasibility();
};

#endif  // ROBUST_CPP_RESOLVE_WITH_REGRET_MINIMIZERS_H
