#include "./../external_code/cxxopts.hpp"
#include "./logging.h"
#include "./model/robust_file_based_program.h"
#include "./solver/pessimization_solver.h"
#include "./solver/resolve_with_regret_minimizers.h"
#include <chrono>

int main(int argc, char *argv[]) {
  cxxopts::Options options("robust_cpp", "Solve robust optimization problems");

  // turn off clang-format as it does not handle the currying well
  // clang-format off
  options.add_options()
    ("m,model", "Nominal model mps file", cxxopts::value<string>())
    ("r,robust", "Robustness file", cxxopts::value<string>())
    ("a,algorithm", "Algorithms: pessimization, oco_opt, oco (TODO)",
     cxxopts::value<string>())
    ("d,debug", "Enable debugging")
    // ("t,iters", "Number of iterations", cxxopts::value<int>())
    ;
  // clang-format on
  options.parse(argc, argv);
  string nominal_file = options["model"].as<string>();
  string robust_file = options["robust"].as<string>();
  string algorithm = options["algorithm"].as<string>();

  if (options.count("debug") > 0) {
    logger->set_level(spdlog::level::debug);
    logger->debug("debugging mode");
  }

  std::unique_ptr<robust_program_dense> rp =
      std::make_unique<robust_file_based_program>(nominal_file, robust_file);
  std::unique_ptr<robust_solver> solver;
  if (algorithm.compare("pessimization") == 0) {
    solver = std::make_unique<pessimization_solver>(rp.get());
  } else {
    solver = std::make_unique<resolve_with_regret_minimizers>(rp.get());
  }

  auto start = std::chrono::high_resolution_clock::now();
  double obj_val = solver->optimize();
  auto finish = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = finish - start;
  int iters = solver->num_iterations();

  printf("Obj: %.3f, Iters: %d, Runtime: %.3f, Status: %d\n", obj_val, iters,
         elapsed.count(), solver->get_status());
}
