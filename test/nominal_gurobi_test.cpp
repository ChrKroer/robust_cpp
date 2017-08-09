#include <Eigen/Core>
#include "../src/model/quadratic_uncertainty_constraint.h"
#include "../src/model/robust_reader.h"
#include "../src/solver/nominal_gurobi.h"
#include "gtest/gtest.h"

class nominal_gurobi_test : public ::testing::Test {
 public:
  virtual void SetUp() {
    nominal_gurobi_ = std::make_unique<nominal_gurobi>(portopt_manual_2);
  }

  virtual void TearDown() {}

  std::string portopt_manual_2 = "../instances/PortOpt_Manual_2.mps";
  std::unique_ptr<nominal_gurobi> nominal_gurobi_;
};

TEST_F(nominal_gurobi_test, update_quadratic_constraint) {
  robust_reader r("../instances/PortOpt_Manual_2.mps",
                  "../instances/PortOpt_Manual_2.json");
  std::unique_ptr<uncertainty_constraint> c = r.next_uncertainty_constraint();
  EXPECT_EQ(uncertainty_constraint::QUADRATIC, c->get_function_type());
  quadratic_uncertainty_constraint *quad_constr =
      dynamic_cast<quadratic_uncertainty_constraint *>(c.get());

  int constraint_id = 0;
  vector_d coeffs(2);
  coeffs << 0.5, 0.5;
  double original_quad_term = 0.01;
  nominal_gurobi_->update_quadratic_constraint(constraint_id, coeffs,
                                               *quad_constr);
}
