#include <Eigen/Core>
#include "gtest/gtest.h"
#include "../src/domain/simplex.h"

TEST(simplex_test, dimension) {
  simplex s2(2);
  EXPECT_EQ(2, s2.dimension());
}

TEST(simplex_test, support2d) {
  simplex s2(2);
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
