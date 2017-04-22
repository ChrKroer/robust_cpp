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
  explicit nominal_gurobi(std::string model_path) {
    grb_env_.set(GRB_IntParam_Threads, 1);
    grb_env_.set(GRB_IntParam_OutputFlag, 0);
    grb_model_ = std::make_unique<GRBModel>(grb_env_, model_path);
  }
  void optimize() override {
    grb_model_->optimize();
  }
  int get_status() override { return grb_model_->get(GRB_IntAttr_Status); }
  int get_objective() override {
    return grb_model_->get(GRB_DoubleAttr_ObjVal);
  }
  int get_var_val(int id) override {
    return grb_model_->getVar(i).get(GRB_DoubleAttr_X);
  }
  void update_constraint(int constraint_id, std::vector<int> var_ids,
                         vector_d coeffs) override {
    assert(coeffs.size() == var_ids.size());
    GRBConstr constr = grb_model_->getConstr(constraint_id);
    for (int i = 0; i < coeffs.size(); i++) {
      GRBVar v = grb_model_->getVar(var_ids[i]);
      grb_model_->chgCoeff(constr, v, coeffs(i));
    }
  }

private:
  GRBEnv grb_env_;
  std::unique_ptr<GRBModel> grb_model_;
};

#endif // ROBUST_CPP_NOMINAL_GUROBI_H
