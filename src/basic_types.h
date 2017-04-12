#ifndef ROBUST_CPP_BASIC_TYPES_H
#define ROBUST_CPP_BASIC_TYPES_H

#include "Eigen/Core"
#include <iostream>

using vector_d = Eigen::VectorXd;

inline void pretty_print(vector_d x) {
  Eigen::IOFormat CommaInitFmt(Eigen::StreamPrecision, Eigen::DontAlignCols,
                               ", ", ", ", "", "", " << ", ";");
  std::cout << x.format(CommaInitFmt) << std::endl;
}

#endif // ROBUST_CPP_BASIC_TYPES_H
