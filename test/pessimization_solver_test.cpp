#include "../src/domain/euclidean_ball.h"
#include "../src/domain/simplex.h"
#include "../src/logging.h"
#include "../src/solver/pessimization_solver.h"
#include "./../src/model/linear_uncertainty_constraint.h"
#include "./../src/model/robust_file_based_program.h"
#include "gtest/gtest.h"
#include <Eigen/Core>
#include <memory>

class pessimization_solver_test : public ::testing::Test {
public:
  virtual void SetUp() {
    rp_afiro = std::make_unique<robust_file_based_program>(filepath_afiro);
    rp_coins = std::make_unique<robust_file_based_program>(filepath_coins);

    ps_afiro = std::make_unique<pessimization_solver>(rp_afiro.get());
    ps_coins = std::make_unique<pessimization_solver>(rp_coins.get());
  }

  virtual void TearDown() {}

  std::string filepath_afiro = "../examples/afiro.mps";
  std::string filepath_coins = "../examples/coins.mps";
  std::unique_ptr<robust_file_based_program> rp_afiro;
  std::unique_ptr<robust_file_based_program> rp_coins;
  std::unique_ptr<pessimization_solver> ps_afiro;
  std::unique_ptr<pessimization_solver> ps_coins;
};

TEST_F(pessimization_solver_test, optimize_deterministic_afiro) {
  double opt = -4.647531429e+02;
  double v0_opt = 80.0;
  double v1_opt = 25.5;

  double val = ps_afiro->optimize();
  vector_d solution = ps_afiro->current_solution();

  ASSERT_NEAR(opt, val, 1e-4);
  ASSERT_NEAR(v0_opt, solution(0), 1e-4);
  ASSERT_NEAR(v1_opt, solution(1), 1e-4);
}

TEST_F(pessimization_solver_test, optimize_deterministic_coins) {
  double opt = -1.134615385e+02;
  double pennies = 0.0;
  double quarters = 53.8461538462;

  double val = ps_coins->optimize();
  vector_d solution = ps_coins->current_solution();

  ASSERT_NEAR(opt, val, 1e-4);
  ASSERT_NEAR(pennies, solution(0), 1e-4);
  ASSERT_NEAR(quarters, solution(3), 1e-4);
}

TEST_F(pessimization_solver_test, optimize_robust_coins) {
  double opt = -113.285;

  std::unique_ptr<robust_file_based_program> rp_coins_robust =
      std::make_unique<robust_file_based_program>(filepath_coins); 
  std::string constraint_name = "Zero";
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
          constraint_name, std::move(b), nominal_coeffs, weights, unc_var_ids);
  rp_coins_robust->add_uncertainty_constraint(std::move(unc_set));

  pessimization_solver ps_coins_robust(rp_coins_robust.get());
  double val = ps_coins_robust.optimize();
  vector_d solution = ps_coins_robust.current_solution();

  //tmp logger->info("val: {}", val);

  EXPECT_NEAR(val, opt, 1e-3);
}
