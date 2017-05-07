//
// Created by Christian Kroer on 4/04/17.
//

#ifndef ROBUST_CPP_PESSIMIZATION_SOLVER_H
#define ROBUST_CPP_PESSIMIZATION_SOLVER_H

#include "../basic_types.h"
#include "./nominal_gurobi.h"
#include "./../model/robust_program_dense.h"
#include "gurobi_c++.h"
#include "./robust_solver.h"

class pessimization_solver : public robust_solver {
public:
  explicit pessimization_solver(const robust_program_dense *rp);
  ~pessimization_solver() {}

  vector_d current_solution() override;
  double optimize() override;
  int num_iterations() override { return num_iterations_; }
  nominal_solver::status get_status() override { return status_; }

private:
  const robust_program *rp_;

  std::unique_ptr<nominal_gurobi> solver_;
  nominal_solver::status status_;
  int num_iterations_;
  double tolerance_ = 1e-6;
};

#endif // ROBUST_CPP_PESSIMIZATION_SOLVER_H
