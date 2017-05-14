#include "../src/domain/euclidean_ball.h"
#include "../src/domain/simplex.h"
#include "../src/logging.h"
#include "../src/solver/trust_region.h"
#include "math.h"
#include "gtest/gtest.h"
#include <Eigen/Core>
#include <memory>

TEST(trust_region_test, init_and_feasible) {
  int dimension = 5;
  vector_d g = vector_d::Constant(dimension, 1.0 / dimension);
  sparse_matrix_d A(dimension, dimension);
  for (int i = 0; i < dimension; i++) {
    A.insert(i, i) = 2.0;
  }
  trust_region ts(g, A);
  ts.optimize();
  EXPECT_EQ(nominal_solver::OPTIMAL, ts.get_status());
  EXPECT_NEAR(1.0, ts.get_solution().norm(), 1e-5);
}

//////////////////////////////////////////////////////////////////
// solve the TRS problem                                        //
// max_xyz 3x+2y+2z + 3x^2 + 3y^2 + 2z^2 - 2xy, ||x,y,z|| <= 1  //
//////////////////////////////////////////////////////////////////
TEST(trust_region_test, finds_optimal) {
  int dimension = 3;
  vector_d g(dimension);
  g << 3, 2, 2;
  sparse_matrix_d A(dimension, dimension);
  A.insert(0, 0) = 3;
  A.insert(1, 1) = 3;
  A.insert(2, 2) = 2;
  A.insert(0, 1) = -1;
  A.insert(1, 0) = -1;

  trust_region ts(g, A);
  ts.optimize();

  double opt = 6.39433;
  vector_d opt_sol(3);
  opt_sol << 0.9202435500487992, 0.05007756141284113, 0.38812844457456025;
  EXPECT_EQ(nominal_solver::OPTIMAL, ts.get_status());
  EXPECT_NEAR(1.0, ts.get_solution().norm(), 1e-5);
  EXPECT_NEAR(opt, ts.get_objective(), 1e-5);
}
