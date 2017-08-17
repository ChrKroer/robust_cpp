#include <Eigen/Core>
#include <Eigen/Eigenvalues>
#include "../src/domain/euclidean_ball.h"
#include "../src/model/quadratic_uncertainty_constraint.h"
#include "../src/model/robust_reader.h"
#include "./../src/logging.h"
#include "gtest/gtest.h"

class quadratic_uncertainty_constraint_test : public ::testing::Test {
 public:
  virtual void SetUp() {
    // matrices get filled in row-wise
    A_0 << 2, 1, 1, 2;
    P_0 << 3, 0, 0, 1;
    P_1 << 2, 0, 1, 1;
    u << 0.5, 0.5;
    x << 0.5, 0.5;

    int dimension = 2;
    double radius = 1.0;
    vector_d center = vector_d::Zero(dimension);
    ball = std::make_unique<euclidean_ball>(dimension, radius, center);
    // q_unc = std::make_unique<quadratic_uncertainty_constraint>(
    //     1, std::move(ball), A_0, {0, 1}, {P_0, P_1}, {2.0}, {"x3"});
    std::vector<int> nom_ind{0, 1};
    std::vector<matrix_d> unc_m{P_0, P_1};
    q_unc = std::make_unique<quadratic_uncertainty_constraint>(
        "test", std::move(ball), A_0, nom_ind, unc_m);
  }

  virtual void TearDown() {}

  matrix_d A_0{2, 2};
  matrix_d P_0{2, 2};
  matrix_d P_1{2, 2};
  vector_d u{2};
  vector_d x{2};

  std::unique_ptr<euclidean_ball> ball;
  std::unique_ptr<quadratic_uncertainty_constraint> q_unc;
};

TEST_F(quadratic_uncertainty_constraint_test, gradient) {
  vector_d b(2);
  b(0) = (A_0 * x).dot(P_0 * x);
  b(1) = (A_0 * x).dot(P_1 * x);

  matrix_d Y(2, 2);
  Y(0, 0) = (P_0 * x).squaredNorm();
  Y(1, 1) = (P_1 * x).squaredNorm();
  Y(0, 1) = (P_0 * x).dot(P_1 * x);
  Y(1, 0) = (P_0 * x).dot(P_1 * x);
  Eigen::SelfAdjointEigenSolver<matrix_d> es(2);
  es.compute(Y);
  double max_eigenval = es.eigenvalues()(1);
  vector_d expected_g = 2 * b + 2 * Y * u - 2 * max_eigenval * u;
  vector_d g = q_unc->gradient(x, u);
  EXPECT_TRUE(g.isApprox(expected_g, 1e-8));
}

TEST_F(quadratic_uncertainty_constraint_test, trs) {
  robust_reader r("../instances/PortOpt_n10_m4_inst_0.mps",
                  "../instances/PortOpt_n10_m4_inst_0.json");
  logger->set_level(spdlog::level::debug);
  std::unique_ptr<uncertainty_constraint> c = r.next_uncertainty_constraint();
  EXPECT_EQ(uncertainty_constraint::QUADRATIC, c->get_function_type());
  quadratic_uncertainty_constraint *quad_c =
      dynamic_cast<quadratic_uncertainty_constraint *>(c.get());

  vector_d nominal_solution(13);
  nominal_solution << -29.4702711, 24.879207, 105.477072, -55.3423176,
      59.0620611, 50.3062603, 4.67509982, -185.931452, 19.0533618, 8.29097847,
      556.334499, 8.02271355, 268.758878;

  const auto maximizer = quad_c->maximizer(nominal_solution);
  EXPECT_NEAR(maximizer.first,
              quad_c->violation_amount(nominal_solution, maximizer.second),
              1e-6);
}
