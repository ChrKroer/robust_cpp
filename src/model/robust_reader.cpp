//
// Created by Christian Kroer on 5/01/17.
//

#include "./robust_reader.h"
#include "./../domain/euclidean_ball.h"
#include "./../logging.h"
#include <fstream>

robust_reader::robust_reader(std::string nominal_file_path,
                             std::string robust_file_path) {
  std::ifstream robust_file_stream(robust_file_path);
  robust_file_stream >> json_;
  grb_env_.set(GRB_IntParam_Threads, 1);
  grb_env_.set(GRB_IntParam_OutputFlag, 0);
  grb_model_ = std::make_unique<GRBModel>(grb_env_, nominal_file_path);
}

bool robust_reader::has_next() { return current_ < json_.size(); }

std::unique_ptr<uncertainty_constraint>
robust_reader::next_uncertainty_constraint() {
  json constraint = json_[current_];
  std::unique_ptr<uncertainty_constraint> constr;
  if (constraint.at("type") == "linear") {
    constr = read_linear_constraint(constraint);
  } else if (constraint.at("type") == "quadratic") {
    constr = read_quadratic_constraint(constraint);
  } else {
    logger->error("constraint type not supported");
  }
  current_++;
  return constr;
}

std::unique_ptr<linear_uncertainty_constraint>
robust_reader::read_linear_constraint(json &c, std::string unc_type) {
  std::string name = c.at("name");
  std::unique_ptr<domain> dom;
  double radius = c.at("uncertainty").at("radius");
  int dimension = c.at("uncertainty").at("dim");
  int constraint_id = c.at("id");
  std::string sign_string = c.at("sense");
  char sense = sign_string.at(0);
  double rhs = c.at("RHS");

  sparse_vector_d nominal_coeffs(grb_model_->get(GRB_IntAttr_NumVars));
  for (json::iterator it = c.at("nominal_coeff").begin();
       it != c.at("nominal_coeff").end(); ++it) {
    nominal_coeffs.coeffRef(std::stoi(it.key())) = it.value()[0];
  }

  std::vector<int> uncertainty_var_ids;
  vector_d weights(dimension);
  for (json::iterator it = c.at("uncertainty").at("data").begin();
       it != c.at("uncertainty").at("data").end(); ++it) {
    weights(uncertainty_var_ids.size()) = it.value()[0];
    uncertainty_var_ids.push_back(std::stoi(it.key()));
  }

  if (unc_type == "L2ball") {
    dom = std::make_unique<euclidean_ball>(dimension, radius);
  } else {
    logger->error("domain type not supported");
  }
  return std::make_unique<linear_uncertainty_constraint>(
      constraint_id, std::move(dom), nominal_coeffs, weights,
      uncertainty_var_ids, rhs, sense);
}

std::unique_ptr<quadratic_uncertainty_constraint>
robust_reader::read_quadratic_constraint(json &c, std::string unc_type) {
  // assert("quadratic".compare(c.at("type")) == 0);
  // assert("L2ball".compare(c.at("uncertainty").at("type")) == 0);
  std::string name = c.at("name");
  int constraint_id = c.at("id");
  std::vector<matrix_d> uncertainty_matrices;
  for (auto &m : c.at("uncertainty").at("data")) {
    uncertainty_matrices.push_back(read_dense_matrix(m));
  }
  matrix_d base_matrix = read_dense_matrix(c.at("base_matrix"));

  double radius = c.at("uncertainty").at("radius");
  int dimension = c.at("uncertainty").at("data").size();
  std::vector<int> vars = c.at("vars");
  std::unique_ptr<domain> dom;
  if (unc_type == "L2ball") {
    dom = std::make_unique<euclidean_ball>(dimension, radius);
  } else {
    logger->error("domain type not supported");
  }

  return std::make_unique<quadratic_uncertainty_constraint>(
      constraint_id, std::move(dom), base_matrix, vars, uncertainty_matrices);
}

matrix_d robust_reader::read_dense_matrix(json &m) {
  // assert("dense".compare(m.at("format")) == 0);
  int nrows = m.at("nrows");
  int ncols = m.at("ncols");
  matrix_d matrix(nrows, ncols);
  for (int row = 0; row < nrows; row++) {
    for (int col = 0; col < nrows; col++) {
      matrix(row, col) = m.at("vals")[row][col];
    }
  }
  return matrix;
}
