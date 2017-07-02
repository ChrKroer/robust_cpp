# robust_cpp

# Setting up dependencies

Install Gurobi, spdlog, cmake, and Eigen. Set environment variables EIGEN_HOME and GUROBI_HOME to point to the respective home directories.

spdlog can easily be installed with `brew install spdlog` on OSX or `apt-get install libspdlog-dev` on Ubuntu. Eigen should also be in homebrew and apt-get.

To get the remaining dependencies, run `get_deps.sh` from the root directory. This downloads the following two header-only librariers:

A `json` library from [this github repo](https://github.com/nlohmann/json).

A `cxxopt` library from [this github repo](https://github.com/jarro2783/cxxopts).

# Building

In order to build from the root directory do:

    mkdir build
    cd build
    cmake ../
    make

