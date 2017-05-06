//
// Created by Christian Kroer on 5/01/17.
//
#include "./nominal_gurobi.h"
#include "./../logging.h"

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

void nominal_gurobi::update_constraint(
    int constraint_id, std::vector<std::pair<int, double>> coeffs) {
  GRBConstr constr = grb_model_->getConstr(constraint_id);
  for (int i = 0; i < coeffs.size(); i++) {
    GRBVar v = grb_model_->getVar(coeffs[i].first);
    grb_model_->chgCoeff(constr, v, coeffs[i].second);
  }
}

void nominal_gurobi::add_linear_constraint(
    std::vector<std::pair<int, double>> coeffs, double rhs) {
  GRBLinExpr newConstr = 0;
  for (int i = 0; i < coeffs.size(); i++) {
    auto var = grb_model_->getVar(coeffs[i].first);
    newConstr += grb_model_->getVar(coeffs[i].first) * coeffs[i].second;
  }
  grb_model_->addConstr(newConstr <= rhs);
}
