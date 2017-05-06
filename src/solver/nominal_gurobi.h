//
// Created by Christian Kroer on 4/22/17.
//

#ifndef ROBUST_CPP_NOMINAL_GUROBI_H
#define ROBUST_CPP_NOMINAL_GUROBI_H

#include "../basic_types.h"
#include "./nominal_solver.h"
#include "gurobi_c++.h"

class nominal_gurobi : public nominal_solver {
public:
  explicit nominal_gurobi(std::string model_path);

  void optimize() override { grb_model_->optimize(); }

  nominal_solver::status get_status() const override;

  double get_objective() override {
    return grb_model_->get(GRB_DoubleAttr_ObjVal);
  }

  double get_var_val(int id) override {
    return grb_model_->getVar(id).get(GRB_DoubleAttr_X);
  }

  void update_constraint(int constraint_id, std::vector<std::pair<int,double>> coeffs) override;

  void add_linear_constraint(std::vector<std::pair<int,double>> coeffs,
                             double rhs);

  double get_rhs(int constraint_id) {
    return grb_model_->getConstr(constraint_id).get(GRB_DoubleAttr_RHS);
  }

  void write_model(string file) {
    grb_model_->write(file);
  }

private:
  GRBEnv grb_env_;
  std::unique_ptr<GRBModel> grb_model_;
};

#endif // ROBUST_CPP_NOMINAL_GUROBI_H
