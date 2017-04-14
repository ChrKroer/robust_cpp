#include "../src/domain/euclidean_ball.h"
#include "../src/domain/simplex.h"
#include "../src/online_convex_optimization/online_mirror_descent.h"
#include "gtest/gtest.h"
#include <Eigen/Core>
#include <memory>

class online_mirror_descent_test : public ::testing::Test {
public:
  virtual void SetUp() {
    dimension = 3;
    simplex_dom = std::make_unique<simplex>(dimension);
    radius = 3;
    center = vector_d(dimension);
    center << 1.0, 2.0, 2.0;
    euclidean_ball_dom =
        std::make_unique<euclidean_ball>(dimension, radius, center);
    md_simplex = std::make_unique<online_mirror_descent>(simplex_dom.get());
    md_euclidean_ball =
        std::make_unique<online_mirror_descent>(euclidean_ball_dom.get());
  }

  virtual void TearDown() {}

  int dimension;
  double radius;
  vector_d center;
  std::unique_ptr<domain> simplex_dom;
  std::unique_ptr<domain> euclidean_ball_dom;
  std::unique_ptr<online_mirror_descent> md_simplex;
  std::unique_ptr<online_mirror_descent> md_euclidean_ball;
};

TEST_F(online_mirror_descent_test, init) {
  vector_d g = vector_d::Constant(dimension, 1.0 / dimension);
  EXPECT_TRUE((g - md_simplex->get_current_solution()).isMuchSmallerThan(1e-6));
}

TEST_F(online_mirror_descent_test, receive_gradient_simplex) {
  vector_d g = vector_d::Zero(dimension);
  g(0) = 1.0;
  md_simplex->receive_gradient(g);
  vector_d expected = vector_d::Constant(dimension, 1.0 / dimension);
  vector_d solution = md_simplex->get_current_solution();
  EXPECT_FALSE((expected - solution).isMuchSmallerThan(1e-6));
  EXPECT_LT(solution(0), 1.0 / dimension);
  for (int i = 1; i < dimension; i++) {
    EXPECT_GT(solution(i), 1.0 / dimension);
  }
}

/////////////////////////////////////////
// solve the optimization problem      //
// max_x 2x_1 +4x_2 + 1x_3, ||x|| <= 3 //
/////////////////////////////////////////
TEST_F(online_mirror_descent_test, receive_gradient_euclidean) {
  vector_d g(dimension);
  g << -2, -4, -1;
  int iters = 80000;
  auto solution_value = [&g](vector_d x) -> double { return (-g).dot(x); };
  for (int i = 0; i < iters; i++) {
    md_euclidean_ball->receive_gradient(g);
    vector_d cur_sol = md_euclidean_ball->get_current_solution();
    EXPECT_NEAR(radius, (cur_sol - center).norm(), 1e-6);
    // pretty_print(cur_sol);
    // std::cout << solution_value(cur_sol) << std::endl << std::endl;
  }
  vector_d sol = md_euclidean_ball->get_current_solution();
  double opt = 25.74772608985767;
  EXPECT_NEAR(opt, solution_value(sol), 1e-5);
  // check that the computed solution is the correct solution
  vector_d opt_sol(dimension);
  opt_sol << 2.3092899183615723, 4.618615388801327, 2.6546846979292194;
  EXPECT_NEAR(opt_sol(0), sol(0), 1e-3);
  EXPECT_NEAR(opt_sol(1), sol(1), 1e-3);
  EXPECT_NEAR(opt_sol(2), sol(2), 1e-3);
}
