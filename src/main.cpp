#include "./../external_code/cxxopts.hpp"

int main(int argc, char *argv[]) {
  cxxopts::Options options("robust_cpp", "Solve robust optimization problems");

  // turn off clang-format as it does not handle the currying well
  // clang-format off
  options.add_options()
    ("m,model", "Nominal model mps file", cxxopts::value<std::string>())
    ("r,robust", "Robustness file", cxxopts::value<std::string>())
    ("a,algorithm", "Algorithms: pessimization, oco_opt, oco (TODO)",
     cxxopts::value<std::string>())
    ;
  // clang-format on
}
