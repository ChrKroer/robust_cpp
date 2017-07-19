//
// Created by Christian Kroer on 5/12/17.
//

#ifndef ROBUST_CPP_TRUST_REGION_H
#define ROBUST_CPP_TRUST_REGION_H

#include "../basic_types.h"
#include "./nominal_solver.h"
#include "gurobi_c++.h"
#include <memory>

////////////////////////////////////////
// Solves the problem max_x g*x + xAx //
// where ||x||<=1                     //
////////////////////////////////////////
class trust_region {
public:
  // explicit trust_region(const vector_d &g, const sparse_matrix_d &A);
  explicit trust_region(const vector_d &g, const matrix_d &A);

  void optimize();

  nominal_solver::status get_status() const;
  vector_d get_solution() const { return final_solution_; }

  double get_objective();

  double get_var_val(int id) {
    return u_[id].get(GRB_DoubleAttr_X);
  }

  void write_model(string file) { grb_model_->write(file); }

private:
  const vector_d &g_;
  const matrix_d &A_;
  GRBEnv grb_env_;
  std::unique_ptr<GRBModel> grb_model_;
  std::vector<GRBVar> u_;
  vector_d final_solution_;

  double max_eigenval_;
  vector_d max_eigenvec_;

  void make_vars();
  void make_constrs();
};

#endif // ROBUST_CPP_TRUST_REGION_H
