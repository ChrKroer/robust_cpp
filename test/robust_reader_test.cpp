#include "../src/model/robust_reader.h"
#include "./../src/domain/euclidean_ball.h"
#include "./../src/model/robust_file_based_program.h"
#include "./../src/model/uncertainty_constraint.h"
#include "./../src/solver/pessimization_solver.h"
#include "gtest/gtest.h"
#include <Eigen/Core>

TEST(robust_reader_test, has_next) {
  robust_reader r("../instances/adlittle.mps",
                  "../instances/adlittle_robust.json");
  EXPECT_TRUE(r.has_next());
}

TEST(robust_reader_test, read_linear_uncertainty_constraint) {
  robust_reader r("../instances/adlittle.mps",
                  "../instances/adlittle_robust.json");
  std::unique_ptr<uncertainty_constraint> c = r.next_uncertainty_constraint();
  int dim = 4;
  int constr_id = 0;

  EXPECT_EQ(uncertainty_constraint::LINEAR, c->get_function_type());
  linear_uncertainty_constraint *lin_c =
      dynamic_cast<linear_uncertainty_constraint *>(c.get());
  EXPECT_EQ(dim, lin_c->dimension());
  EXPECT_EQ(constr_id, lin_c->get_constraint_id());

  // check that var indices are correct
  EXPECT_EQ(lin_c->uncertainty_variable_ids()[0], 60);
  EXPECT_EQ(lin_c->uncertainty_variable_ids()[1], 61);
  EXPECT_EQ(lin_c->uncertainty_variable_ids()[2], 76);
  EXPECT_EQ(lin_c->uncertainty_variable_ids()[3], 77);

  // check that nominal var coefficients are correct
  EXPECT_EQ(lin_c->nominal_coeffs().coeffRef(0), 0.506);
  EXPECT_EQ(lin_c->nominal_coeffs().coeffRef(1), 0.638);
  EXPECT_EQ(lin_c->nominal_coeffs().coeffRef(2), -1.0);
  EXPECT_EQ(lin_c->nominal_coeffs().coeffRef(48), -0.247);

  // read next unc constraint
  dim = 5;
  constr_id = 5;
  c = r.next_uncertainty_constraint();
  EXPECT_EQ(uncertainty_constraint::LINEAR, c->get_function_type());
  lin_c = dynamic_cast<linear_uncertainty_constraint *>(c.get());
  EXPECT_EQ(dim, lin_c->dimension());
  EXPECT_EQ(constr_id, lin_c->get_constraint_id());
}

TEST(robust_reader_test, read_whole_linear_instance) {
  std::unique_ptr<robust_file_based_program> rp =
      std::make_unique<robust_file_based_program>(
          "../instances/adlittle.mps", "../instances/adlittle_robust.json");
  pessimization_solver ps(rp.get());
  double val = ps.optimize();
  EXPECT_GT(val, -464.753);
}
