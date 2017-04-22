#include "../src/domain/euclidean_ball.h"
#include "../src/domain/simplex.h"
#include "../src/logging.h"
#include "../src/solver/resolve_with_regret_minimizers.h"
#include "./../src/model/euclidean_ball_uncertainty_constraint.h"
#include "./../src/model/robust_linear_program.h"
#include "gtest/gtest.h"
#include <Eigen/Core>
#include <memory>

class resolve_with_regret_minimizers_test : public ::testing::Test {
public:
  virtual void SetUp() {
    rp_afiro = std::make_unique<robust_linear_program>(filepath_afiro);
    rp_coins = std::make_unique<robust_linear_program>(filepath_coins);

    solver_afiro =
        std::make_unique<resolve_with_regret_minimizers>(rp_afiro.get());
    solver_coins =
        std::make_unique<resolve_with_regret_minimizers>(rp_coins.get());
  }

  virtual void TearDown() {}

  std::string filepath_afiro = "../examples/afiro.mps";
  std::string filepath_coins = "../examples/coins.mps";
  std::unique_ptr<robust_linear_program> rp_afiro;
  std::unique_ptr<robust_linear_program> rp_coins;
  std::unique_ptr<resolve_with_regret_minimizers> solver_afiro;
  std::unique_ptr<resolve_with_regret_minimizers> solver_coins;
};

TEST_F(resolve_with_regret_minimizers_test, optimize_deterministic_afiro) {
  double opt = -4.647531429e+02;
  double v0_opt = 80.0;
  double v1_opt = 25.5;

  double val = solver_afiro->optimize();
  vector_d solution = solver_afiro->current_strategy();

  ASSERT_NEAR(opt, val, 1e-4);
  ASSERT_NEAR(v0_opt, solution(0), 1e-4);
  ASSERT_NEAR(v1_opt, solution(1), 1e-4);
}

TEST_F(resolve_with_regret_minimizers_test, optimize_deterministic_coins) {
  double opt = -1.134615385e+02;
  double pennies = 0.0;
  double quarters = 53.8461538462;

  double val = solver_coins->optimize();
  vector_d solution = solver_coins->current_strategy();

  ASSERT_NEAR(opt, val, 1e-4);
  ASSERT_NEAR(pennies, solution(0), 1e-4);
  ASSERT_NEAR(quarters, solution(3), 1e-4);
}

TEST_F(resolve_with_regret_minimizers_test, optimize_robust_coins) {
  double opt = -1.133333333e+02;
  double pennies = 0.0;
  double dimes = 133.333333333;
  double quarters = 0.0;

  std::unique_ptr<robust_linear_program> rp_coins_robust =
      std::make_unique<robust_linear_program>(filepath_coins);
  int constraint_id = 0;
  vector_d center(9);
  center << 0.06, 3.8, 2.1, 6.2, 7.2, -1.0, 0.0, 0.0, 0.0;
  std::unique_ptr<euclidean_ball_uncertainty_constraint> unc_set =
      std::make_unique<euclidean_ball_uncertainty_constraint>(
          9, 0.001, center, uncertainty_constraint::LINEAR);
  rp_coins_robust->add_uncertainty_constraint(constraint_id,
                                              std::move(unc_set));

  resolve_with_regret_minimizers solver_coins_robust(rp_coins_robust.get());
  double val = solver_coins_robust.optimize();
  vector_d solution = solver_coins_robust.current_strategy();

  logger->info("val: {}", val);

  ASSERT_GT(val, opt);
  // ASSERT_NEAR(pennies, solution(0), 1e-4);
  // ASSERT_NEAR(dimes, solution(2), 1e-4);
  // ASSERT_NEAR(quarters, solution(3), 1e-4);
}