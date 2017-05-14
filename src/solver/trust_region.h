//
// Created by Christian Kroer on 5/12/17.
//

#ifndef ROBUST_CPP_TRUST_REGION_H
#define ROBUST_CPP_TRUST_REGION_H

#include "../basic_types.h"
#include "./nominal_solver.h"
#include "gurobi_c++.h"

////////////////////////////////////////
// Solves the problem max_x g*x + xAx //
// where ||x||<=1                     //
////////////////////////////////////////
class trust_region {
public:
  explicit trust_region(vector_d g, sparse_matrix_d A);

  void optimize();

  nominal_solver::status get_status() const;
  vector_d get_solution() const { return final_solution_; }

  double get_objective();

  double get_var_val(int id) {
    return u[id].get(GRB_DoubleAttr_X);
  }

  void write_model(string file) { grb_model_->write(file); }

private:
  GRBEnv grb_env_;
  std::unique_ptr<GRBModel> grb_model_;
  std::vector<GRBVar> u;
  vector_d final_solution_;
  vector_d g_;
  sparse_matrix_d A_;

  double max_eigenval_;
  vector_d max_eigenvec_;

  void make_vars();
  void make_constrs();
};

#endif // ROBUST_CPP_TRUST_REGION_H
