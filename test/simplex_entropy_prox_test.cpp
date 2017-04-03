#include "../src/domain/simplex_entropy_prox.h"
#include "gtest/gtest.h"
#include <Eigen/Core>

TEST(simplex_entropy_prox_test, center) {
  simplex_entropy_prox p(2);
  vector_d v = p.center();
  EXPECT_NEAR(0.5, v(0), 1e-6);
  EXPECT_NEAR(0.5, v(1), 1e-6);
}

TEST(simplex_entropy_prox_test, prox_mapping) {
  int dim = 5;
  simplex_entropy_prox p(dim);
  double prox_weight = 9.0;

  auto ent = [&dim, &prox_weight](vector_d x) -> double {
    return prox_weight * (x.array().log().matrix().dot(x) + std::log(dim));
  };
  auto smooth_f = [&ent](vector_d g, vector_d x) -> double {
    return g.dot(x) + ent(x);
  };
  for (int i = 0; i < 20; i++) {
    vector_d g = vector_d::Random(dim);
    std::tuple<double, vector_d> t = p.mapping(1.0, g, prox_weight);
    double prox_val = std::get<0>(t);
    vector_d prox_map = std::get<1>(t);
    EXPECT_NEAR(smooth_f(g, prox_map), prox_val, 1e-6);
  }
}
