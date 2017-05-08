#include "../src/model/robust_reader.h"
#include "./../src/domain/euclidean_ball.h"
#include "./../src/logging.h"
#include "./../src/model/robust_file_based_program.h"
#include "./../src/model/uncertainty_constraint.h"
#include "./../src/solver/pessimization_solver.h"
#include "./../src/solver/resolve_with_regret_minimizers.h"
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

bool test_program(std::string name) {
  std::unique_ptr<robust_file_based_program> rp =
      std::make_unique<robust_file_based_program>(
          "../instances/" + name + ".mps",
          "../instances/" + name + "_robust.json");
  // logger->set_level(spdlog::level::debug);
  pessimization_solver ps(rp.get());
  resolve_with_regret_minimizers rs(rp.get());
  double val_ps = ps.optimize();
  double val_rs = rs.optimize();
  // logger->info("pessimization: {}", val_ps);
  // logger->info("rms: {}", val_rs);
  if (val_ps > 0) {
    return 0.999 * val_ps <= val_rs || val_rs <= 1.001 * val_ps;
  } else {
    return 0.999 * val_ps >= val_rs || val_rs <= 1.001 * val_ps;
  }
}

TEST(robust_reader_test, read_whole_linear_instance) {
  EXPECT_TRUE(test_program("adlittle"));
  EXPECT_TRUE(test_program("afiro"));
  EXPECT_TRUE(test_program("agg"));
  EXPECT_TRUE(test_program("agg2"));
  EXPECT_TRUE(test_program("agg3"));
}
