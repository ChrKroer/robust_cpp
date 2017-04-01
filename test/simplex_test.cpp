#include "../src/domain/simplex.h"
#include "gtest/gtest.h"
#include <Eigen/Core>

TEST(simplex_test, dimension) {
  simplex s2(2);
  EXPECT_EQ(2, s2.dimension());
}

TEST(simplex_test, support2d) {
  simplex s2(2);
  vector_d g = vector_d::Zero(2);
  g(0) = 1.0;
  std::tuple<double, vector_d> t = s2.support(g);
  EXPECT_EQ(1, std::get<0>(t));
  vector_d r = std::get<1>(t);
  EXPECT_EQ(1, r(0));
  EXPECT_EQ(0, r(1));

  g(1) = 2.0;
  t = s2.support(g);
  EXPECT_EQ(2, std::get<0>(t));
  r = std::get<1>(t);
  EXPECT_EQ(1, r(1));
  EXPECT_EQ(0, r(0));
}
