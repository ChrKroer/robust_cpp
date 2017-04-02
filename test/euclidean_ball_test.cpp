#include "../src/domain/euclidean_ball.h"
#include "gtest/gtest.h"
#include <Eigen/Core>

class euclidean_ball_test : public ::testing::Test {
public:
  virtual void SetUp() {
    dimension = 3;
    radius = 1.0;
    center = vector_d::Zero(dimension);
    ball = std::make_unique<euclidean_ball>(dimension, radius, center);
  }

  virtual void TearDown() {}

  int dimension;
  double radius;
  vector_d center;
  std::unique_ptr<euclidean_ball> ball;
};

TEST_F(euclidean_ball_test, dimension) {
  EXPECT_EQ(3, ball->dimension());
}

TEST_F(euclidean_ball_test, support3d) {
  vector_d g = vector_d::Zero(dimension);
  g(0) = 1.0;
  std::tuple<double, vector_d> t = ball->support(g);
  EXPECT_EQ(1, std::get<0>(t));
  vector_d r = std::get<1>(t);
  EXPECT_EQ(1, r(0));
  EXPECT_EQ(0, r(1));
  EXPECT_EQ(0, r(2));

  g(1) = 2.0;
  t = ball->support(g);
  double expected_val = g.norm();
  EXPECT_EQ(expected_val, std::get<0>(t));
  vector_d argmax = g / g.norm();
  vector_d solution = std::get<1>(t);
  EXPECT_TRUE((argmax-solution).isMuchSmallerThan(1e-6));
}
