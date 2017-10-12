#include <chrono>
#include <future>
#include <thread>
#include "./../external_code/cxxopts.hpp"
#include ".//../external_code/json.hpp"
#include "./logging.h"
#include "./model/robust_file_based_program.h"
#include "./solver/pessimization_solver.h"
#include "./solver/resolve_with_regret_minimizers.h"

int main(int argc, char *argv[]) {
  using json = nlohmann::json;
  cxxopts::Options options("robust_cpp", "Solve robust optimization problems");

  // turn off clang-format as it does not handle the currying well
  // clang-format off
  options.add_options()
    ("m,model", "Nominal model mps file", cxxopts::value<string>())
    ("r,robust", "Robustness file", cxxopts::value<string>(), "")
    ("a,algorithm", "Algorithms: pessimization, oco_opt, oco (TODO)",
     cxxopts::value<string>())
    ("regret_minimizer", "Algorithms: mirror_descent, dual_averaging, adam, ftpl",
     cxxopts::value<string>())
    ("v,verbosity", "Output level", cxxopts::value<int>())
    ("s,stepsize", "", cxxopts::value<double>())
    ("f,feasibility_tol", "", cxxopts::value<double>())
    ("t,when_to_average", "When to start averaging solutions",
     cxxopts::value<int>())
    ;
  // clang-format on
  options.parse(argc, argv);
  string nominal_file = options["model"].as<string>();
  string robust_file = options["robust"].as<string>();
  size_t lastindex = nominal_file.find_last_of(".");
  string instance = nominal_file.substr(0, lastindex);
  if (robust_file.compare("") == 0) {
    robust_file = instance + ".json";
  }
  string algorithm = options["algorithm"].as<string>();

  if (options.count("verbosity") > 0) {
    int log_level = options["verbosity"].as<int>();
    switch (log_level) {
      case 0:
        logger->set_level(spdlog::level::trace);
        break;
      case 1:
        logger->set_level(spdlog::level::debug);
        break;
      case 2:
        logger->set_level(spdlog::level::info);
        break;
      case 3:
        logger->set_level(spdlog::level::warn);
        break;
      case 4:
        logger->set_level(spdlog::level::err);
        break;
      case 5:
        logger->set_level(spdlog::level::critical);
        break;
      default:
        logger->set_level(spdlog::level::off);
    }
  } else {
    logger->set_level(spdlog::level::off);
  }

  double feasibility_tol = 1e-2;
  if (options.count("feasibility_tol") > 0) {
    feasibility_tol = options["feasibility_tol"].as<double>();
    logger->debug("feasibility tol: {}", feasibility_tol);
  }

  logger->debug("reading problem");
  std::unique_ptr<robust_program_dense> rp =
      std::make_unique<robust_file_based_program>(nominal_file, robust_file);
  logger->debug("making solver");
  std::shared_ptr<robust_solver> solver;
  resolve_with_regret_minimizers::regret_minimizer rms =
      resolve_with_regret_minimizers::regret_minimizer::
          project_gradient_descent;
  std::string rms_name = "projected_gradient_descent";
  if (algorithm == "pessimization") {
    logger->debug("pessimization solver");
    solver = std::make_shared<pessimization_solver>(rp.get(), feasibility_tol);
  } else {
    logger->debug("regret solver");
    int when_to_average = 2;
    double stepsize = 1.0;
    if (options.count("regret_minimizer") > 0 &&
        options["regret_minimizer"].as<string>().compare("dual_averaging") ==
            0) {
      rms = resolve_with_regret_minimizers::regret_minimizer::dual_averaging;
      rms_name = "dual_averaging";
    } else if (options.count("regret_minimizer") > 0 &&
               options["regret_minimizer"].as<string>().compare("adam") == 0) {
      rms = resolve_with_regret_minimizers::regret_minimizer::adam;
      rms_name = "adam";
    } else if (options.count("regret_minimizer") > 0 &&
               options["regret_minimizer"].as<string>().compare("ftpl") == 0) {
      rms = resolve_with_regret_minimizers::regret_minimizer::ftpl;
      rms_name = "ftpl";
    }
    if (options.count("when_to_average") > 0) {
      when_to_average = options["when_to_average"].as<int>();
    }
    if (options.count("stepsize") > 0) {
      stepsize = options["stepsize"].as<double>();
    }
    solver = std::make_shared<resolve_with_regret_minimizers>(
        rp.get(), when_to_average, rms, stepsize, feasibility_tol);
  }

  logger->debug("solving");

  std::future<double> future =
      std::async(std::launch::async, &robust_solver::optimize, solver);

  auto start = std::chrono::high_resolution_clock::now();
  std::future_status status = future.wait_for(std::chrono::minutes(5));
  if (status != std::future_status::ready) {
    json output;
    output["status"] = "timeout";
    output["algorithm"] = algorithm;
    output["instance"] = instance;
    std::cout << output.dump(4) << std::endl;
    exit(0);
  }
  // double obj_val = solver->optimize();
  auto finish = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = finish - start;
  int iters = solver->num_iterations();

  // logger->debug(
  //     "Obj: %.3f, Iters: %d, Runtime: %.3f, Status: %d, Algorithm: %s\n",
  //     obj_val, iters, elapsed.count(), solver->get_status(),
  //     algorithm.c_str());
  json output;
  if (algorithm == "regret") {
    output["algorithm"] = algorithm + "_" + rms_name;
  } else {
    output["algorithm"] = algorithm;
  }
  output["feasibility_tol"] = feasibility_tol;
  output["grb_runtimes"] = solver->solve_times();
  output["instance"] = instance;
  output["iterations"] = iters;
  output["max_violations"] = solver->max_violations();
  output["objective"] = future.get();
  output["runtime"] = elapsed.count();
  output["solver_status"] = solver->get_status();
  output["status"] = "complete";
  if (algorithm == "regret") {
    output["stopped_with_current"] =
        (dynamic_cast<resolve_with_regret_minimizers *>(solver.get()))
            ->stopped_with_current();
  }
  std::cout << output.dump(4) << std::endl;
}
