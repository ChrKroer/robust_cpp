//
// Created by Christian Kroer on 3/26/17.
//

#include "./simplex.h"

Simplex::Simplex(int dimension) : Domain(dimension) {

}

Simplex::~Simplex() {

}

std::tuple<double, vector_t> Simplex::support(
    vector_t const& g) {
  Eigen::Index idx;
  double v = g.maxCoeff(&idx);
  vector_t response = Eigen::VectorXd::Zero(g.innerSize());
  response(idx) = 1.0;

  return  std::make_tuple(v, response);
}

