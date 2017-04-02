//
// Created by Christian Kroer on 3/26/17.
//

#include "./simplex.h"
#include "./simplex_entropy_prox.h"

simplex::simplex(int dimension)
    : domain(), dimension_(dimension),
      prox_(std::unique_ptr<simplex_entropy_prox>(
          new simplex_entropy_prox(dimension))) {}

std::tuple<double, vector_d> simplex::support(vector_d const &g) const {
  Eigen::Index idx;
  double v = g.maxCoeff(&idx);
  vector_d response = vector_d::Zero(g.innerSize());
  response(idx) = 1.0;

  return std::make_tuple(v, response);
}
