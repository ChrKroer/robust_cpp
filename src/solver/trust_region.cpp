//
// Created by Christian Kroer on 5/12/17.
//
#include "./trust_region.h"
#include "./../logging.h"
#include "Eigen/Eigenvalues"
#include <cmath>

trust_region::trust_region(vector_d g, sparse_matrix_d A)
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
    final_solution_(i) = u[i].get(GRB_DoubleAttr_X);
  }
  double low = 0.0;
  double high = 1.0;
  // write_model("test.lp");
  // logger->info("final sol: {}", eigen_to_string(final_solution_));
  // logger->info("eigenvec: {}", eigen_to_string(max_eigenvec_));
  while (std::abs((final_solution_ + (high - low) / 2 * max_eigenvec_).norm() -
                  1) > 1e-6) {
    // logger->info("norm: {}",
    //              (final_solution_ + (high - low) / 2 *
    //              max_eigenvec_).norm());
    if ((final_solution_ + (high - low) / 2 * max_eigenvec_).norm() < 1.0) {
      low = (high - low) / 2;
    } else {
      high = (high - low) / 2;
    }
  }

  final_solution_ += (high - low) / 2 * max_eigenvec_;
}

double trust_region::get_objective() {
  return g_.dot(final_solution_) + (A_ * final_solution_).dot(final_solution_);
}

void trust_region::make_vars() {
  // GRBVar t =
  //     grb_model_->addVar(-GRB_INFINITY, GRB_INFINITY, 1.0, GRB_CONTINUOUS,
  //     "t");
  // grb_model_->setObjective(t, GRB_MAXIMIZE);
  for (int i = 0; i < g_.size(); i++) {
    u.push_back(grb_model_->addVar(-1, 1, 1.0, GRB_CONTINUOUS,
                                   "u" + std::to_string(i)));
  }
  grb_model_->update();
}

void trust_region::make_constrs() {
  // norm ball constraint
  GRBQuadExpr norm_ball;
  for (int i = 0; i < g_.size(); i++) {
    norm_ball += u[i] * u[i];
  }
  grb_model_->addQConstr(norm_ball <= 1);

  // objective constraint
  GRBQuadExpr obj;
  // add linear term
  for (int i = 0; i < g_.size(); i++) {
    obj += u[i] * g_[i];
    obj += -max_eigenval_ * u[i] * u[i];
  }
  // add quadratic term
  for (int col = 0; col < A_.outerSize(); ++col) {
    for (sparse_matrix_d::InnerIterator it(A_, col); it; ++it) {
      int row = it.row();
      obj += u[row] * u[col] * it.value();
    }
  }
  grb_model_->setObjective(obj, GRB_MAXIMIZE);
  // grb_model_->addConstr(t <= obj);
  grb_model_->update();
}

nominal_solver::status trust_region::get_status() const {
  int status = grb_model_->get(GRB_IntAttr_Status);
  if (status == GRB_OPTIMAL) {
    return nominal_solver::OPTIMAL;
  } else if (status == GRB_INFEASIBLE) {
    return nominal_solver::INFEASIBLE;
  } else {
    return nominal_solver::UNDEFINED;
  }
}
