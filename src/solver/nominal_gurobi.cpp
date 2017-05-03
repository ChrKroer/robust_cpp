//
// Created by Christian Kroer on 5/01/17.
//
#include "./nominal_gurobi.h"

nominal_gurobi::nominal_gurobi(std::string model_path) {
  grb_env_.set(GRB_IntParam_Threads, 1);
  grb_env_.set(GRB_IntParam_OutputFlag, 0);
  grb_model_ = std::make_unique<GRBModel>(grb_env_, model_path);
}

nominal_solver::status nominal_gurobi::get_status() const {
  int status = grb_model_->get(GRB_IntAttr_Status);
  if (status == GRB_OPTIMAL) {
    return nominal_solver::OPTIMAL;
  } else if (status == GRB_INFEASIBLE) {
    return nominal_solver::INFEASIBLE;
  } else {
    return nominal_solver::UNDEFINED;
  }
}

void nominal_gurobi::update_constraint(int constraint_id,
                                       std::vector<int> var_ids,
                                       vector_d coeffs) {
  assert(coeffs.size() == var_ids.size());
  GRBConstr constr = grb_model_->getConstr(constraint_id);
  for (int i = 0; i < coeffs.size(); i++) {
    GRBVar v = grb_model_->getVar(var_ids[i]);
    grb_model_->chgCoeff(constr, v, coeffs(i));
  }
}

void nominal_gurobi::add_linear_constraint(std::vector<int> var_ids,
                                           vector_d coeffs, double rhs) {
  assert(coeffs.size() == var_ids.size());
  GRBLinExpr newConstr = 0;
  for (int i = 0; i < var_ids.size(); i++) {
    newConstr += grb_model_->getVar(var_ids[i]) * coeffs(i);
  }
  grb_model_->addConstr(newConstr <= rhs);
}
