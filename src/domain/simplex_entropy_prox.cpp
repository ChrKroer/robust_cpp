#include "./simplex_entropy_prox.h"

simplex_entropy_prox::simplex_entropy_prox(int dimension)
    : dimension_(dimension) {}

vector_d simplex_entropy_prox::center() const {
  vector_d g = vector_d::Zero(dimension());
  std::tuple<double, vector_d> t = mapping(0.0, g, 1.0);
  return std::get<1>(t);
}

std::tuple<double, vector_d>
simplex_entropy_prox::bregman(double alpha, const vector_d &g, double beta,
                              const vector_d &y) const {
  double offset;
  if (alpha < 0.0) {
    offset = g.maxCoeff();
  } else {
    offset = g.maxCoeff();
  }

  vector_d z = (-alpha / beta * (g.array() - offset)).exp();
  z = z.cwiseProduct(y);
  double Z = z.array().sum();
  z /= Z;

  return std::make_pair(alpha * offset - beta * std::log(Z), z);
}

std::tuple<double, vector_d> simplex_entropy_prox::mapping(double alpha,
                                                           const vector_d &g,
                                                           double beta) const {
  double offset;
  if (alpha < 0.0) {
    offset = g.maxCoeff();
  } else {
    offset = g.maxCoeff();
  }

  vector_d z = (-alpha / beta * (g.array() - offset)).exp();
  z /= dimension();
  double Z = z.array().sum();
  z /= Z;

  return std::make_pair(alpha * offset - beta * std::log(Z), z);
}
