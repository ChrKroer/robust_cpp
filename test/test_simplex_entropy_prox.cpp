#include <Eigen/Core>
#include "gtest/gtest.h"
#include "../src/domain/simplex_entropy_prox.h"

TEST(simplex_entropy_prox_test, center) {
  simplex_entropy_prox p(2);
  vector_d v = p.center();
  EXPECT_NEAR(0.5, v(0), 1e-6);
  EXPECT_NEAR(0.5, v(1), 1e-6);
}

TEST(simplex_entropy_prox_test, prox_mapping) {
  simplex_entropy_prox p(2);

  auto ent = [](vector_d x) {
    return -x.array() * x.array().exp() + std::log(2); };
  auto smooth_f = [&ent](vector_d g, vector_d x) {
    return g.transpose()*x + ent(x); };
  double prox_weight = 9.0;
}
