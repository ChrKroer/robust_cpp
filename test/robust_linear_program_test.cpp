#include "../src/model/robust_linear_program.h"
#include "gtest/gtest.h"
#include <Eigen/Core>
#include <memory>

class robust_linear_program_test : public ::testing::Test {
public:
  virtual void SetUp() {
    rp_afiro = std::make_unique<robust_linear_program>(filepath_afiro);
    rp_coins = std::make_unique<robust_linear_program>(filepath_coins);
  }

  virtual void TearDown() {}

  std::string filepath_afiro = "../examples/afiro.mps";
  std::string filepath_coins = "../examples/coins.mps";
  std::unique_ptr<robust_linear_program> rp_afiro;
  std::unique_ptr<robust_linear_program> rp_coins;
};

TEST_F(robust_linear_program_test, dimension) {
  int dimension_coins = 9;
  int dimension_afiro = 32;
  EXPECT_EQ(dimension_coins, rp_coins->dimension());
  EXPECT_EQ(dimension_afiro, rp_afiro->dimension());
}

TEST_F(robust_linear_program_test, num_constraints) {
  int num_constraints_coins = 4;
  int num_constraints_afiro = 27;
  EXPECT_EQ(num_constraints_coins, rp_coins->num_constraints());
  EXPECT_EQ(num_constraints_afiro, rp_afiro->num_constraints());
}
