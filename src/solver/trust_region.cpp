//
// Created by Christian Kroer on 5/12/17.
//
#include "./trust_region.h"
#include <cmath>
#include "./../logging.h"
#include "Eigen/Eigenvalues"

trust_region::trust_region(const vector_d &g, const matrix_d &A)
    : g_(g), A_(A), final_solution_(g.size()) {
  grb_env_.set(GRB_IntParam_Threads, 1);
  grb_env_.set(GRB_IntParam_OutputFlag, 0);
  grb_model_ = std::make_unique<GRBModel>(grb_env_);

  Eigen::SelfAdjointEigenSolver<matrix_d> es(g_.size());
  es.compute(A);
  max_eigenval_ = es.eigenvalues()(g.size() - 1);
  max_eigenvec_ = es.eigenvectors().col(g.size() - 1);

  make_vars();
  make_constrs();
}

void trust_region::optimize() {
  grb_model_->optimize();
  // send to boundary
  for (int i = 0; i < g_.size(); i++) {
    final_solution_(i) = u_[i].get(GRB_DoubleAttr_X);
  }
  double low = 0.0;
  double high = 2.0;
  double mid = low + (high - low) / 2;
  //not necessarily solution between zero and one, |ev| = 1 - solution between 0 and 2?
  while (std::abs((final_solution_ + mid * max_eigenvec_).norm() - 1) > 1e-6) {
    if ((final_solution_ + mid * max_eigenvec_).norm() < 1.0) {
      low = low + (high - low) / 2;
    } else {
      high = low + (high - low) / 2;
    }
    mid = low + (high - low) / 2;
  }

  final_solution_ += (high - low) / 2 * max_eigenvec_;
}

double trust_region::get_objective() {
  return g_.dot(final_solution_) + (A_ * final_solution_).dot(final_solution_);
}

void trust_region::make_vars() {
  for (int i = 0; i < g_.size(); i++) {
    u_.push_back(grb_model_->addVar(-1, 1, 1.0, GRB_CONTINUOUS,
                                    "u" + std::to_string(i)));
  }
  grb_model_->update();
}

void trust_region::make_constrs() {
  // norm ball constraint
  GRBQuadExpr norm_ball;
  for (int i = 0; i < g_.size(); i++) {
    norm_ball += u_[i] * u_[i];
  }
  grb_model_->addQConstr(norm_ball <= 1);

  // objective constraint
  GRBQuadExpr obj;
  // add linear term
  for (int i = 0; i < g_.size(); i++) {
    obj += u_[i] * g_[i];
    obj += -max_eigenval_ * u_[i] * u_[i];
  }
  // add quadratic term
  // for (int col = 0; col < A_.outerSize(); ++col) {
  //   for (sparse_matrix_d::InnerIterator it(A_, col); it; ++it) {
  //     const int row = it.row();
  //     obj += u[row] * u[col] * it.value();
  //   }
  // }
  for (int col = 0; col < A_.cols(); col++) {
    for (int row = 0; row < A_.rows(); row++) {
      obj += u_[row] * u_[col] * A_(row, col);
    }
  }
  grb_model_->setObjective(obj, GRB_MAXIMIZE);
  // grb_model_->addConstr(t <= obj);
  grb_model_->update();
}

nominal_solver::status trust_region::get_status() const {
  const int status = grb_model_->get(GRB_IntAttr_Status);
  if (status == GRB_OPTIMAL) {
    return nominal_solver::OPTIMAL;
  } else if (status == GRB_INFEASIBLE) {
    return nominal_solver::INFEASIBLE;
  } else {
    return nominal_solver::UNDEFINED;
  }
}
