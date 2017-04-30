#include "../src/domain/euclidean_ball.h"
#include "../src/domain/simplex.h"
#include "../src/logging.h"
#include "../src/online_convex_optimization/online_mirror_descent.h"
#include "math.h"
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
    center << 3.0, 4.0, 4.0;
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
  int iters = 100;
  auto solution_value = [&g](vector_d x) -> double { return (-g).dot(x); };
  for (int i = 0; i < iters; i++) {
    md_euclidean_ball->receive_gradient(g);
    vector_d current = md_euclidean_ball->get_current_solution();
    ASSERT_LT((current - center).norm(), radius + 1e-6);
  }
  vector_d sol = md_euclidean_ball->get_current_solution();
  double opt = 39.74772641589396;
  EXPECT_NEAR(opt, solution_value(sol), 1e-5);
  // check that the computed solution is the unique correct solution
  vector_d opt_sol(dimension);
  opt_sol << 4.30890760611214, 6.618846220558929, 4.654526321433962;
  EXPECT_NEAR(opt_sol(0), sol(0), 1e-3);
  EXPECT_NEAR(opt_sol(1), sol(1), 1e-3);
  EXPECT_NEAR(opt_sol(2), sol(2), 1e-3);
}

//////////////////////////////////////////////////////////////////////
// solve the optimization problem                                   //
// min_{x,y,z} 2x +4y + 1z - log(5x + 2y + 3z), ||[x y z]||_2 <= 3  //
//////////////////////////////////////////////////////////////////////
TEST_F(online_mirror_descent_test, min_log) {
  int iters = 2000;
  vector_d lin_g(dimension);
  lin_g << 2.0, 4.0, 2.0;
  vector_d log_coeffs(dimension);
  log_coeffs << 3.0, 2.0, 3.0;
  auto gradient = [&lin_g, &log_coeffs](vector_d x) -> vector_d {
    double log_val = log_coeffs.dot(x);
    return lin_g - log_coeffs / log_val;
  };
  auto solution_value = [&lin_g, &log_coeffs](vector_d x) -> double {
    return lin_g.dot(x) - log(log_coeffs.dot(x));
  };
  for (int i = 0; i < iters; i++) {
    vector_d current = md_euclidean_ball->get_current_solution();
    md_euclidean_ball->receive_gradient(gradient(current));
    ASSERT_LT((current - center).norm(), radius + 1e-6);
  }
  vector_d sol = md_euclidean_ball->get_current_solution();
  double opt = 12.478476891081868;
  EXPECT_NEAR(opt, solution_value(sol), 1e-3);
  // check that the computed solution is the unique correct solution
  vector_d opt_sol(dimension);
  opt_sol << 1.8263873911818438, 1.500949062720722, 2.8263883912465766;
  EXPECT_NEAR(opt_sol(0), sol(0), 1e-3);
  EXPECT_NEAR(opt_sol(1), sol(1), 1e-3);
  EXPECT_NEAR(opt_sol(2), sol(2), 1e-3);
}
