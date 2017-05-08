#include "../src/domain/euclidean_ball.h"
#include "../src/domain/simplex.h"
#include "../src/logging.h"
#include "../src/solver/resolve_with_regret_minimizers.h"
#include "./../src/model/linear_uncertainty_constraint.h"
#include "./../src/model/robust_file_based_program.h"
#include "gtest/gtest.h"
#include <Eigen/Core>
#include <memory>

class resolve_with_regret_minimizers_test : public ::testing::Test {
public:
  virtual void SetUp() {
    rp_afiro = std::make_unique<robust_file_based_program>(filepath_afiro);
    rp_coins = std::make_unique<robust_file_based_program>(filepath_coins);

    solver_afiro =
        std::make_unique<resolve_with_regret_minimizers>(rp_afiro.get());
    solver_coins =
        std::make_unique<resolve_with_regret_minimizers>(rp_coins.get());
  }

  virtual void TearDown() {}

  std::string filepath_afiro = "../examples/afiro.mps";
  std::string filepath_coins = "../examples/coins.mps";
  std::unique_ptr<robust_file_based_program> rp_afiro;
  std::unique_ptr<robust_file_based_program> rp_coins;
  std::unique_ptr<resolve_with_regret_minimizers> solver_afiro;
  std::unique_ptr<resolve_with_regret_minimizers> solver_coins;
};

TEST_F(resolve_with_regret_minimizers_test, optimize_robust_coins) {
  double opt = -113.285;
  double pennies = 0.0;
  double dimes = 133.333333333;
  double quarters = 0.0;

  // logger->set_level(spdlog::level::debug);
  std::unique_ptr<robust_file_based_program> rp_coins_robust =
      std::make_unique<robust_file_based_program>(filepath_coins);
  int constraint_id = 0;
  double radius = 0.001;
  vector_d center = vector_d::Zero(6);
  vector_d weights = vector_d::Constant(6, 1.0);
  std::unique_ptr<euclidean_ball> b =
      std::make_unique<euclidean_ball>(6, radius, center);
  std::vector<int> unc_var_ids = {0, 1, 2, 3, 4, 5};
  vector<double> nominal = {0.06, 3.8, 2.1, 6.2, 7.2, -1.0};
  sparse_vector_d nominal_coeffs(6);
  for (int i = 0; i < center.size(); i++) {
    nominal_coeffs.coeffRef(i) = nominal[i];
  }
  std::unique_ptr<linear_uncertainty_constraint> unc_set =
      std::make_unique<linear_uncertainty_constraint>(
          constraint_id, std::move(b), nominal_coeffs, weights, unc_var_ids);
  rp_coins_robust->add_uncertainty_constraint(std::move(unc_set));

  resolve_with_regret_minimizers solver_coins_robust(rp_coins_robust.get());
  double val = solver_coins_robust.optimize();
  vector_d solution = solver_coins_robust.current_solution();

  logger->info("val: {}", val);

  EXPECT_NEAR(val, opt, 1e-3);
}
