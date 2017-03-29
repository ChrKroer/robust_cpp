//
// Created by Christian Kroer on 3/26/17.
//

#include "./simplex.h"
#include "./simplex_entropy_prox.h"

simplex::simplex(int dimension) :
    domain(dimension, std::unique_ptr<simplex_entropy_prox>(
        new simplex_entropy_prox(dimension))) {

}

simplex::~simplex() {

}

std::tuple<double, vector_t> simplex::support(
    vector_t const& g) {
  Eigen::Index idx;
  double v = g.maxCoeff(&idx);
  vector_t response = vector_t::Zero(g.innerSize());
  response(idx) = 1.0;

  return  std::make_tuple(v, response);
}

