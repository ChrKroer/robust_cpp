#include "./simplex_entropy_prox.h"

simplex_entropy_prox::simplex_entropy_prox(int dimension) : prox(dimension) {

}

simplex_entropy_prox::~simplex_entropy_prox() {

}

vector_t simplex_entropy_prox::center() {
  std::tuple<double, vector_t> t = mapping(
      0.0, vector_t::Zero(dimension()), 1.0);
  return std::get<1>(t);
}

std::tuple<double, vector_t> simplex_entropy_prox::bregman(
    double alpha, vector_t g, double beta, vector_t y) {
  double offset;
  if (alpha < 0.0) {
    offset = g.maxCoeff();
  } else {
    offset = g.maxCoeff();
  }

  vector_t z = (-alpha/beta * (g.array() - offset)).exp();
  z = z.cwiseProduct(y);
  double Z = z.array().sum();
  z /= Z;

  return std::make_pair(alpha*offset - beta*std::log(Z), z);
}

std::tuple<double, vector_t> simplex_entropy_prox::mapping(
    double alpha, vector_t g, double beta) {
  double offset;
  if (alpha < 0.0) {
    offset = g.maxCoeff();
  } else {
    offset = g.maxCoeff();
  }

  vector_t z = (-alpha/beta * (g.array() - offset)).exp();
  z /= dimension();
  double Z = z.array().sum();
  z /= Z;

  return std::make_pair(alpha*offset - beta*std::log(Z), z);
}
