#ifndef ROBUST_CPP_BASIC_TYPES_H
#define ROBUST_CPP_BASIC_TYPES_H

#include "Eigen/Core"
#include <iostream>

#define TRACE_MSG                                                              \
  fprintf(stderr, "[%s:%d] %s here I am\n", __FILE__, __LINE__,                 \
          __PRETTY_FUNCTION__)

using vector_d = Eigen::VectorXd;

inline void pretty_print(vector_d x) {
  Eigen::IOFormat CommaInitFmt(Eigen::StreamPrecision, Eigen::DontAlignCols,
                               ", ", ", ", "", "", " << ", ";");
  std::cout << x.format(CommaInitFmt) << std::endl;
}

#endif // ROBUST_CPP_BASIC_TYPES_H
