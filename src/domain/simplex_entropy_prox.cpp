#include "./simplex_entropy_prox.h"

SimplexEntropyProx::SimplexEntropyProx(int dimension) : Prox(dimension) {

}

~SimplexEntropyProx::SimplexEntropyProx() {

}

Eigen::VectorXd SimplexEntropyProx::center() {
  std::map<double, vector_t> t = mapping(
      0.0, Eigen::VectorXd::Zero(dimension()), 1.0);
  return std::get<0>(t);
}

std::tuple<double, vector_t> SimplexEntropyProx::bregman(double alpha, Eigen::VectorXd g, double beta, Eigen::VectorXd y=NULL) {
  double offset;
  if (alpha < 0.0) {
    offset = g.maxCoeff();
  } else {
    offset = g.maxCoeff();
  }

  Eigen::VectorXd z = (-alpha/beta * (g - offset)).array().exp();
  z = z.cwiseProduct(y);
  double Z = z.array().sum();
  z /= Z;

  return std::make_pair(alpha*offset - beta*std::log(Z), z);
}

std::tuple<double, vector_t> SimplexEntropyProx::mapping(double alpha, Eigen::VectorXd g, double beta) {
  double offset;
  if (alpha < 0.0) {
    offset = g.maxCoeff();
  } else {
    offset = g.maxCoeff();
  }

  Eigen::VectorXd z = (-alpha/beta * (g - offset)).array().exp();
  z /= dimension();
  double Z = z.array().sum();
  z /= Z;

  return std::make_pair(alpha*offset - beta*std::log(Z), z);
}
