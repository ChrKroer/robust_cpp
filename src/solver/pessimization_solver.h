//
// Created by Christian Kroer on 4/04/17.
//

#ifndef ROBUST_CPP_PESSIMIZATION_SOLVER_H
#define ROBUST_CPP_PESSIMIZATION_SOLVER_H

#include "../basic_types.h"
#include "./../model/robust_program.h"
#include "gurobi_c++.h"

class pessimization_solver {
public:
  explicit pessimization_solver(const robust_program *rp);
  ~pessimization_solver() {}

  vector_d current_strategy();
  double optimize();

private:
  void add_uncertainty_constraint(int constraint_id, vector_d coeff);

  const robust_program *rp_;

  GRBEnv env_;
  GRBModel grb_model_;
};

#endif // ROBUST_CPP_PESSIMIZATION_SOLVER_H
