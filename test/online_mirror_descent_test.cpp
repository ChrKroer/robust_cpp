#include "../src/domain/simplex.h"
#include "../src/online_convex_optimization/online_mirror_descent.h"
#include "gtest/gtest.h"
#include <Eigen/Core>
#include <memory>

class online_mirror_descent_test : public ::testing::Test {
public:
  virtual void SetUp() {
    dimension = 5;
    std::unique_ptr<domain> s = std::make_unique<simplex>(dimension);
    md = std::make_unique<online_mirror_descent>(std::move(s));
  }

  virtual void TearDown() {}

  int dimension;
  std::unique_ptr<online_mirror_descent> md;
};

TEST_F(online_mirror_descent_test, init) {
  vector_d g = vector_d::Constant(dimension, 1.0 / 5);
  EXPECT_TRUE((g-md->get_current_solution()).isMuchSmallerThan(1e-6));
}

TEST_F(online_mirror_descent_test, receive_gradient) {
  vector_d g = vector_d::Zero(dimension);
  g(0) = 1.0;
  md->receive_gradient(g);
  pretty_print(md->get_current_solution());
  vector_d expected = vector_d::Constant(dimension, 1.0 / dimension);
  vector_d solution = md->get_current_solution();
  EXPECT_FALSE((expected-solution).isMuchSmallerThan(1e-6));
  EXPECT_LT(solution(0), 1.0 / dimension);
  for (int i = 1; i < dimension; i++) {
    EXPECT_GT(solution(i), 1.0 / dimension);
  }
}
