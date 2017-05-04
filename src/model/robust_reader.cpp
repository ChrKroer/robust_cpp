//
// Created by Christian Kroer on 5/01/17.
//

#include "./robust_reader.h"
#include "./../domain/euclidean_ball.h"
#include "./../logging.h"
#include <fstream>

robust_reader::robust_reader(std::string robust_file_path) {
  std::ifstream robust_file_stream(robust_file_path);
  robust_file_stream >> json_;
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
  int constr_id = c.at("id");
  std::vector<int> uncertainty_var_ids;
  vector_d center(dimension);
  for (json::iterator it = c.at("uncertainty").at("data").begin();
       it != c.at("uncertainty").at("data").end(); ++it) {
    center(uncertainty_var_ids.size()) = it.value()[0];
    uncertainty_var_ids.push_back(std::stoi(it.key()));
  }
  std::vector<std::pair<int, double>> nominal_coeffs;
  for (json::iterator it = c.at("nominal_coeff").begin();
       it != c.at("nominal_coeff").end(); ++it) {
    nominal_coeffs.push_back(
        std::make_pair(std::stoi(it.key()), it.value()[0]));
  }
  if (unc_type == "L2ball") {
    dom = std::make_unique<euclidean_ball>(dimension, radius, center);
  } else {
    logger->error("domain type not supported");
  }
  return std::make_unique<linear_uncertainty_constraint>(
      constr_id, std::move(dom), nominal_coeffs, uncertainty_var_ids);
}

std::unique_ptr<quadratic_uncertainty_constraint>
robust_reader::read_quadratic_constraint(json &c, std::string unc_type) {}
