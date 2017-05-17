//
// Created by Christian Kroer on 5/01/17.
//

#ifndef ROBUST_CPP_ROBUST_READER_H
#define ROBUST_CPP_ROBUST_READER_H

#include "../domain/domain.h"
#include "./uncertainty_constraint.h"
#include "./linear_uncertainty_constraint.h"
#include "./quadratic_uncertainty_constraint.h"
#include "./../../external_code/json.hpp"
#include "gurobi_c++.h"
#include <vector>
#include <unordered_map>

using json = nlohmann::json;

class robust_reader {
public:
  robust_reader(std::string nominal_file_path, std::string robust_file_path);
  ~robust_reader() {}

  bool has_next();
  std::unique_ptr<uncertainty_constraint> next_uncertainty_constraint();
private:
  int current_ = 0;
  json json_;

  GRBEnv grb_env_;
  std::unique_ptr<GRBModel> grb_model_;

  std::unique_ptr<linear_uncertainty_constraint> read_linear_constraint(json&, std::string unc_type="L2ball");
  std::unique_ptr<quadratic_uncertainty_constraint> read_quadratic_constraint(json&, std::string unc_type="L2ball");
  matrix_d read_dense_matrix(json &m);
};

#endif // ROBUST_CPP_ROBUST_READER_H
