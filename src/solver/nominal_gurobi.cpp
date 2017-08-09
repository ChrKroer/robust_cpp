//
// Created by Christian Kroer on 5/01/17.
//
#include "./../logging.h"
#include "./nominal_gurobi.h"

nominal_gurobi::nominal_gurobi(const std::string &model_path) {
  grb_env_.set(GRB_IntParam_Threads, 1);
  grb_env_.set(GRB_IntParam_OutputFlag, 0);
  grb_env_.set(
      GRB_IntParam_DualReductions,
      0);  // for debugging, lets gurobi distinguish infeasible vs unbounded
  grb_model_ = std::make_unique<GRBModel>(grb_env_, model_path);
}

nominal_solver::status nominal_gurobi::get_status() const {
  const int status = grb_model_->get(GRB_IntAttr_Status);
  if (status == GRB_OPTIMAL) {
    return nominal_solver::OPTIMAL;
  } else if (status == GRB_INFEASIBLE) {
    return nominal_solver::INFEASIBLE;
  } else {
    return nominal_solver::UNDEFINED;
  }
}

void nominal_gurobi::update_constraint(const int constraint_id,
                                       const vector_d &unc_coeffs,
                                       const uncertainty_constraint &unc) {
  if (unc.get_function_type() == uncertainty_constraint::LINEAR) {
    const linear_uncertainty_constraint &lin_unc =
        dynamic_cast<const linear_uncertainty_constraint &>(unc);
    update_linear_constraint(constraint_id, unc_coeffs, lin_unc);
  } else {
    logger->error("constraint type not yet supported.");
    TRACE;
    std::exit(1);
  }
}

void nominal_gurobi::update_linear_constraint(
    const int constraint_id, const vector_d &unc_coeffs,
    const linear_uncertainty_constraint &unc) {
  const sparse_vector_d coeffs = unc.get_full_coeffs(unc_coeffs);
  const GRBConstr constr = grb_model_->getConstr(constraint_id);
  const std::vector<int> &var_ids = unc.uncertainty_variable_ids();
  for (int i = 0; i < var_ids.size(); i++) {
    const GRBVar var = grb_model_->getVar(var_ids[i]);
    const double coeff = unc.get_coeff(i, unc_coeffs[i]);
    grb_model_->chgCoeff(constr, var, coeff);
  }
}

void nominal_gurobi::update_quadratic_constraint(
    const int constraint_id, const vector_d &coeffs,
    const quadratic_uncertainty_constraint &unc) {
  // constraint to update
  const GRBQConstr constr = grb_model_->getQConstrs()[constraint_id];
  const GRBQuadExpr quad_row_expr = grb_model_->getQCRow(constr);
  const GRBLinExpr lin_row_expr = quad_row_expr.getLinExpr();
  const matrix_d m = unc.get_matrix_instantiation(coeffs);
  // new quadratic term
  matrix_d m2 = m.transpose() * m;
  for (int col = 0; col < m2.cols(); col++) {
    for (int row = 0; row < m2.rows(); row++) {
      int nominal_col_id = unc.get_nominal_id(col);
      int nominal_row_id = unc.get_nominal_id(row);
      // TODO: update entry here
    }
  }
}

void nominal_gurobi::add_constraint(const vector_d &unc_coeffs,
                                    const uncertainty_constraint &unc) {
  if (unc.get_function_type() == uncertainty_constraint::LINEAR) {
    const linear_uncertainty_constraint &lin_unc =
        dynamic_cast<const linear_uncertainty_constraint &>(unc);
    add_linear_constraint(unc_coeffs, lin_unc);
  } else {
    const quadratic_uncertainty_constraint &quad_unc =
        dynamic_cast<const quadratic_uncertainty_constraint &>(unc);
    add_quadratic_constraint(unc_coeffs, quad_unc);
  }
}

void nominal_gurobi::add_linear_constraint(
    const vector_d &unc_coeffs, const linear_uncertainty_constraint &unc) {
  const sparse_vector_d coeffs = unc.get_full_coeffs(unc_coeffs);
  GRBLinExpr newConstr;
  for (sparse_vector_d::InnerIterator it(coeffs); it; ++it) {
    const GRBVar var = grb_model_->getVar(it.index());
    const double val = it.value();
    newConstr += var * val;
  }
  for (int i = 0; i < unc.get_certain_var().first.size(); i++) {
    newConstr += unc.get_certain_var().first[i] *
                 grb_model_->getVarByName(unc.get_certain_var().second[i]);
  }

  grb_model_->addConstr(newConstr <= unc.get_rhs());
}

// this should arguably not care about nominal IDs and instead receive a
// sparse
// matrix from quadratic_uncertainty_constraint
void nominal_gurobi::add_quadratic_constraint(
    const vector_d &coeffs, const quadratic_uncertainty_constraint &unc) {
  const matrix_d m = unc.get_matrix_instantiation(coeffs);
  matrix_d m2 = m.transpose() * m;

  GRBQuadExpr expr;
  for (int col = 0; col < m2.cols(); col++) {
    for (int row = 0; row < m2.rows(); row++) {
      int nominal_col_id = unc.get_nominal_id(col);
      int nominal_row_id = unc.get_nominal_id(row);
      expr += grb_model_->getVar(nominal_col_id) *
              grb_model_->getVar(nominal_row_id) * m2(row, col);
    }
  }

  for (int i = 0; i < unc.get_certain_var().first.size(); i++) {
    expr += unc.get_certain_var().first[i] *
            grb_model_->getVarByName(unc.get_certain_var().second[i]);
  }
  grb_model_->addQConstr(expr <= unc.get_rhs());
}

double nominal_gurobi::get_quad_coeff(
    const quadratic_uncertainty_constraint &unc, int index1, int index2) const {
  const GRBQConstr constr = grb_model_->getQConstrs()[unc.get_constraint_id()];
  const GRBQuadExpr quad_row_expr = grb_model_->getQCRow(constr);
  for (int i = 0; i < quad_row_expr.size(); i++) {
    if ((quad_row_expr.getVar1(i).sameAs(grb_model_->getVar(index1)) &&
         quad_row_expr.getVar2(i).sameAs(grb_model_->getVar(index2))) ||
        (quad_row_expr.getVar2(i).sameAs(grb_model_->getVar(index1)) &&
         quad_row_expr.getVar1(i).sameAs(grb_model_->getVar(index2)))) {
      return quad_row_expr.getCoeff(i);
    }
  }
  return 0;
}
