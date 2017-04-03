#include <Eigen/Core>
#include "gtest/gtest.h"
#include "../src/domain/euclidean_ball_l2_prox.h"

class euclidean_ball_l2_prox_test : public ::testing::Test {
public:
  virtual void SetUp() {
    dimension = 3;
    radius = 1.0;
    center = vector_d::Zero(dimension);
    prox = std::make_unique<euclidean_ball_l2_prox>(dimension, radius, center);
  }

  virtual void TearDown() {}

  int dimension;
  double radius;
  vector_d center;
  std::unique_ptr<euclidean_ball_l2_prox> prox;
};

// TEST_F(euclidean_ball_l2_prox_test, center) {
//   vector_d v = prox->center();
//   EXPECT_NEAR(1.0/dimension, v(0), 1e-6);
//   EXPECT_NEAR(1.0/dimension, v(1), 1e-6);
// }

TEST_F(euclidean_ball_l2_prox_test, prox_mapping) {
  double prox_weight = 9.0;

  auto smooth_f = [&prox_weight](vector_d g, vector_d x) -> double {
    return g.dot(x) + (prox_weight/2) * x.squaredNorm();
  };
  for (int i = 0; i < 20; i++) {
    vector_d g = vector_d::Random(dimension);
    std::tuple<double, vector_d> t = prox->mapping(1.0, g, prox_weight);
    double prox_val = std::get<0>(t);
    vector_d prox_map = std::get<1>(t);
    EXPECT_NEAR(smooth_f(g, prox_map), prox_val, 1e-6);
  }
}
