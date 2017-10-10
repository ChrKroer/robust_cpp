#include <Eigen/Core>
#include "../src/model/robust_reader.h"
#include "./../src/domain/euclidean_ball.h"
#include "./../src/logging.h"
#include "./../src/model/robust_file_based_program.h"
#include "./../src/model/uncertainty_constraint.h"
#include "./../src/online_convex_optimization/online_dual_averaging.h"
#include "./../src/online_convex_optimization/online_mirror_descent.h"
#include "./../src/solver/pessimization_solver.h"
#include "./../src/solver/resolve_with_regret_minimizers.h"
#include "gtest/gtest.h"

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
  std::string constraint_name = "....01";

  EXPECT_EQ(uncertainty_constraint::LINEAR, c->get_function_type());
  linear_uncertainty_constraint *lin_c =
      dynamic_cast<linear_uncertainty_constraint *>(c.get());
  EXPECT_EQ(dim, lin_c->dimension());
  EXPECT_EQ(constraint_name, lin_c->get_constraint_name());

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
  constraint_name = "....06";
  c = r.next_uncertainty_constraint();
  EXPECT_EQ(uncertainty_constraint::LINEAR, c->get_function_type());
  lin_c = dynamic_cast<linear_uncertainty_constraint *>(c.get());
  EXPECT_EQ(dim, lin_c->dimension());
  EXPECT_EQ(constraint_name, lin_c->get_constraint_name());
}

bool test_program(std::string name) {
  std::unique_ptr<robust_file_based_program> rp =
      std::make_unique<robust_file_based_program>(
          "../instances/" + name + ".mps",
          "../instances/" + name + "_robust.json");
  logger->set_level(spdlog::level::info);
  pessimization_solver ps(rp.get());
  resolve_with_regret_minimizers rs(rp.get());
  double val_ps = ps.optimize();
  double val_rs = rs.optimize();
  // //tmp logger->info("pessimization: {}", val_ps);
  // //tmp logger->info("rms: {}", val_rs);
  if (val_ps > 0) {
    return 0.999 * val_ps <= val_rs || val_rs <= 1.001 * val_ps;
  } else {
    return 0.999 * val_ps >= val_rs || val_rs <= 1.001 * val_ps;
  }
}

TEST(robust_reader_test, read_whole_linear_instance) {
  // EXPECT_TRUE(test_program("adlittle"));
  // EXPECT_TRUE(test_program("afiro"));
  // EXPECT_TRUE(test_program("agg"));
}

TEST(robust_reader_test, read_quadratic_uncertainty_constraint) {
  robust_reader r("../instances/Manual_2.mps", "../instances/Manual_2.json");
  // logger->set_level(spdlog::level::debug);
  r.next_uncertainty_constraint();
  std::string constraint_name = "qc1";
  std::unique_ptr<uncertainty_constraint> c = r.next_uncertainty_constraint();

  EXPECT_EQ(uncertainty_constraint::QUADRATIC, c->get_function_type());
  quadratic_uncertainty_constraint *quad_c =
      dynamic_cast<quadratic_uncertainty_constraint *>(c.get());

  EXPECT_EQ(constraint_name, quad_c->get_constraint_name());

  // check that first uncertain matrix is ok
  EXPECT_EQ(quad_c->uncertain_matrices()[0](0, 0), 0.5);

  // check that base matrix is correct
  EXPECT_EQ(quad_c->base_matrix()(0, 0), 2);
}

// TODO: use test_program instead and delete this
bool test_port_opt(std::string name) {
  std::unique_ptr<robust_file_based_program> rp =
      std::make_unique<robust_file_based_program>(
          "../instances/" + name + ".mps", "../instances/" + name + ".json");
  pessimization_solver ps(rp.get());
  double val_ps = ps.optimize();
  return true;
}
TEST(robust_reader_test, read_whole_quadratic_instance) {
  // EXPECT_TRUE(test_port_opt("PortOpt_n20_m8_inst_0"));
  // EXPECT_TRUE(test_port_opt("Manual_2"));
}

void solve_rms_instance(vector_d x) {
  std::string name = "../instances/PortOpt_n5_m2_inst_11";
  robust_reader r(name + ".mps", name + ".json");
  std::string constraint_name = "uncertain_risk";
  std::unique_ptr<uncertainty_constraint> c = r.next_uncertainty_constraint();
  EXPECT_EQ(uncertainty_constraint::QUADRATIC, c->get_function_type());
  quadratic_uncertainty_constraint *unc =
      dynamic_cast<quadratic_uncertainty_constraint *>(c.get());
  EXPECT_EQ(constraint_name, unc->get_constraint_name());

  double stepsize = 0.51;
  online_dual_averaging rms(unc->get_domain(), stepsize);
  vector_d unc_current = rms.get_current_solution();
  for (int outer = 0; outer < 1; outer++) {
    vector_d g = unc->gradient(x, unc_current);
    logger->debug("g: {}, norm: {}", eigen_to_string(g), g.norm());
    double violation_amount = unc->violation_amount(x, unc_current);
    logger->debug("rms violation: {}, norm: {}", violation_amount,
                  unc_current.norm());
    for (int inner = 0; inner < 100; inner++) {
      unc_current = rms.get_current_solution();
      g = unc->gradient(x, unc_current);
      rms.receive_gradient(-g);
      unc_current = rms.get_current_solution();
      unc->push_to_boundary(&unc_current, x);
      rms.set_solution(unc_current);
    }

    violation_amount = unc->violation_amount(x, unc_current);
    logger->debug("rms violation: {}, norm: {}", violation_amount,
                  unc_current.norm());
  }
  std::pair<double, vector_d> maximizer = unc->maximizer(x);
  double maximizer_violation_amount =
      unc->violation_amount(x, maximizer.second);
  double oco_violation_amount = unc->violation_amount(x, unc_current);
  logger->debug("Unc solution: {}",
                eigen_to_string(rms.get_current_solution()));
  logger->debug("Max solution: {}", eigen_to_string(maximizer.second));

  EXPECT_NEAR(maximizer_violation_amount, oco_violation_amount, 1e-4);
}

TEST(robust_reader_test, online_gradient_and_maximizer_agree) {
  vector_d x(8);
  x << 78.2, 39.2, 14.1, -81.2, -49.3, 272.0, 1.5, 132.0;
  solve_rms_instance(x);
  x << 78.23061795803599, 39.26103165287722, 14.10296934888073,
      -81.28236941312122, -49.31224954667272, 272.0350191359996,
      1.570115942212218, 132.021178437736;
  solve_rms_instance(x);
  x << 78.22525656059172, 39.2704123263927, 14.10294592185835,
      -81.29043379385129, -49.30818101499148, 272.0591557910213,
      1.569464357561121, 132.0339218801552;
  solve_rms_instance(x);
  x << 78.2250274988037, 39.27122449008507, 14.10280907300159,
      -81.29136263023815, -49.30769843165703, 272.0619294372643,
      1.569394959553707, 132.0353801918376;
  solve_rms_instance(x);
}
