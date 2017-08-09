//
// Created by Christian Kroer on 4/22/17.
//

#ifndef ROBUST_CPP_NOMINAL_GUROBI_H
#define ROBUST_CPP_NOMINAL_GUROBI_H

#include "../basic_types.h"
#include "./../model/linear_uncertainty_constraint.h"
#include "./nominal_solver.h"
#include "gurobi_c++.h"
#include "../model/quadratic_uncertainty_constraint.h"

class nominal_gurobi : public nominal_solver {
public:
  explicit nominal_gurobi(const std::string &model_path);

  void optimize() override { grb_model_->update(); grb_model_->write("debug.lp"); grb_model_->optimize(); }

  nominal_solver::status get_status() const override;

  double get_objective() override {
    return grb_model_->get(GRB_DoubleAttr_ObjVal);
  }

  double get_var_val(const int id) override {
    return grb_model_->getVar(id).get(GRB_DoubleAttr_X);
  }

  void update_constraint(const int constraint_id, const vector_d &coeffs,
                         const uncertainty_constraint &unc) override;

  void
  update_linear_constraint(const int constraint_id, const vector_d &coeffs,
                           const linear_uncertainty_constraint &unc);

  void
  update_quadratic_constraint(const int constraint_id, const vector_d &coeffs,
                           const quadratic_uncertainty_constraint &unc);

  void add_constraint(const vector_d &coeffs,
                      const uncertainty_constraint &unc) override;

  void add_linear_constraint(const vector_d &coeffs,
                             const linear_uncertainty_constraint &unc);
  void add_quadratic_constraint(const vector_d &coeffs,
                             const quadratic_uncertainty_constraint &unc);

  double get_rhs(const int constraint_id) {
    return grb_model_->getConstr(constraint_id).get(GRB_DoubleAttr_RHS);
  }

  void write_model(const std::string &file) { grb_model_->write(file); }
  double get_variable_value(const std::string var) override {
    return grb_model_->getVarByName(var).get(GRB_DoubleAttr_X);
  }
private:
  GRBEnv grb_env_;
  std::unique_ptr<GRBModel> grb_model_;
};

#endif // ROBUST_CPP_NOMINAL_GUROBI_H
