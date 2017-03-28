//
// Created by Christian Kroer on 3/26/17.
//

#ifndef ROBUST_CPP_PROX_H
#define ROBUST_CPP_PROX_H

#include <Eigen/Core>
#include <tuple>

class Prox {
 public:
  explicit Prox(int dimension) : dimension_(dimension) {}
  ~Prox() {}

  int dimension() { return dimension_; }

  virtual Eigen::VectorXd center() = 0;
  virtual std::tuple<double, vector_t> bregman(
      double alpha, Eigen::VectorXd g, double beta, Eigen::VectorXd y) = 0;
  virtual std::tuple<double, vector_t> mapping(
      double alpha, Eigen::VectorXd g, double beta) = 0;

 private:
  int dimension_;
};

#endif //ROBUST_CPP_PROX_H
