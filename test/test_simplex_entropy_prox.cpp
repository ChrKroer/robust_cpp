#include <Eigen/Core>
#include "gtest/gtest.h"
#include "../src/domain/simplex_entropy_prox.h"

TEST(SimplexEntropyProxTest, diameter) {
  SimplexEntropyProx p(2);
  EXPECT_NEAR(0.6931472, s2.diameter(), 1e-6);
}

TEST(SimplexEntropyProxTest, support2d) {
  SimplexEntropyProx p(2);

  prox_weight = 9.0;

  vector_t g = vector_t::Zero(2);
  g(0) = 1.0;
  std::tuple<double, vector_t> t = s2.support(g);
  EXPECT_EQ(1, std::get<0>(t));
  vector_t r = std::get<1>(t);
  EXPECT_EQ(1, r(0));
  EXPECT_EQ(0, r(1));

  g(1) = 2.0;
  t = s2.support(g);
  EXPECT_EQ(2, std::get<0>(t));
  r = std::get<1>(t);
  EXPECT_EQ(1, r(1));
  EXPECT_EQ(0, r(0));
}
