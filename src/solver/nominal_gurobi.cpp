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

void nominal_gurobi::update_constraint(int constraint_id,
                                       const vector_d &unc_coeffs,
                                       const uncertainty_constraint &unc) {
  if (unc.get_function_type() == uncertainty_constraint::LINEAR) {
    const linear_uncertainty_constraint &lin_unc =
        dynamic_cast<const linear_uncertainty_constraint &>(unc);
    update_linear_constraint(constraint_id, unc_coeffs, lin_unc);
  } else {
    logger->error("constraint type not yet supported.");
    TRACE_MSG;
    std::exit(1);
  }
}

void nominal_gurobi::update_linear_constraint(
    int constraint_id, const vector_d &unc_coeffs,
    const linear_uncertainty_constraint &unc) {
  sparse_vector_d coeffs = unc.get_full_coeffs(unc_coeffs);
  GRBConstr constr = grb_model_->getConstr(constraint_id);
  const std::vector<int> &var_ids = unc.uncertainty_variable_ids();
  for (int i = 0; i < var_ids.size(); i++) {
    auto var = grb_model_->getVar(var_ids[i]);
    double coeff = unc.get_coeff(i, unc_coeffs[i]);
    grb_model_->chgCoeff(constr, var, coeff);
  }
}

void nominal_gurobi::add_constraint(const vector_d &unc_coeffs,
                                    const uncertainty_constraint &unc) {
  if (unc.get_function_type() == uncertainty_constraint::LINEAR) {
    const linear_uncertainty_constraint &lin_unc =
        dynamic_cast<const linear_uncertainty_constraint &>(unc);
    add_linear_constraint(unc_coeffs, lin_unc);
  } else {
    logger->error("constraint type not yet supported.");
    std::exit(1);
  }
}

void nominal_gurobi::add_linear_constraint(
    const vector_d &unc_coeffs, const linear_uncertainty_constraint &unc) {
  sparse_vector_d coeffs = unc.get_full_coeffs(unc_coeffs);
  GRBLinExpr newConstr = 0;
  for (sparse_vector_d::InnerIterator it(coeffs); it; ++it) {
    auto var = grb_model_->getVar(it.index());
    newConstr += var * it.value();
  }
  grb_model_->addConstr(newConstr <= unc.get_rhs());
}
