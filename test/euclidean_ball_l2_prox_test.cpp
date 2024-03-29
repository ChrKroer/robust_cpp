#include "../src/domain/euclidean_ball_l2_prox.h"
#include "gtest/gtest.h"
#include <Eigen/Core>

class euclidean_ball_l2_prox_test : public ::testing::Test {
public:
  virtual void SetUp() {
    dimension = 3;
    radius = 1.0;
    center = vector_d::Zero(dimension);
    prox = std::make_unique<euclidean_ball_l2_prox>(dimension, radius, center);
    off_zero_center = vector_d(dimension);
    off_zero_center << 1.0, 2.0, 2.0;
    off_zero_prox = std::make_unique<euclidean_ball_l2_prox>(dimension, radius,
                                                             off_zero_center);
  }

  virtual void TearDown() {}

  int dimension;
  double radius;
  vector_d center;
  vector_d off_zero_center;
  std::unique_ptr<euclidean_ball_l2_prox> prox;
  std::unique_ptr<euclidean_ball_l2_prox> off_zero_prox;
};

// TEST_F(euclidean_ball_l2_prox_test, center) {
//   vector_d v = prox->center();
//   EXPECT_NEAR(1.0/dimension, v(0), 1e-6);
//   EXPECT_NEAR(1.0/dimension, v(1), 1e-6);
// }

TEST_F(euclidean_ball_l2_prox_test, prox_mapping) {
  double prox_weight = 9.0;
  auto smooth_f = [&prox_weight](vector_d g, vector_d x) -> double {
    return g.dot(x) + (prox_weight / 2) * x.squaredNorm();
  };
  for (int i = 0; i < 20; i++) {
    vector_d g = vector_d::Random(dimension);
    std::tuple<double, vector_d> t = prox->mapping(1.0, g, prox_weight);
    double prox_val = std::get<0>(t);
    vector_d prox_map = std::get<1>(t);
    EXPECT_NEAR(smooth_f(g, prox_map), prox_val, 1e-6);
  }
}

TEST_F(euclidean_ball_l2_prox_test, center) {
  vector_d zero_prox_center = prox->center();
  EXPECT_NEAR(0, zero_prox_center.norm(), 1e-6);
  vector_d off_zero_prox_center = off_zero_prox->center();
  EXPECT_TRUE((off_zero_prox_center - off_zero_center).isMuchSmallerThan(1e-6));
}
