#ifndef ROBUST_CPP_BASIC_TYPES_H
#define ROBUST_CPP_BASIC_TYPES_H

#include "Eigen/Core"
#include <iostream>
#include <vector>

#define TRACE_MSG                                                              \
  fprintf(stderr, "[%s:%d] %s here I am\n", __FILE__, __LINE__,                 \
          __PRETTY_FUNCTION__)

using vector_d = Eigen::VectorXd;

inline void pretty_print(vector_d x) {
  Eigen::IOFormat CommaInitFmt(Eigen::StreamPrecision, Eigen::DontAlignCols,
                               ", ", ", ", "", "", " << ", ";");
  std::cout << x.format(CommaInitFmt) << std::endl;
}

inline std::string eigen_to_string(vector_d x) {
  Eigen::IOFormat CommaInitFmt(Eigen::StreamPrecision, Eigen::DontAlignCols,
                               ", ", ", ", "", "", " << ", ";");
  std::stringstream ss;
  ss << x.format(CommaInitFmt);
  return ss.str();
}

inline std::string sparse_vector_string(std::vector<std::pair<int,double>> &x) {
  std::stringstream ss;
  for (auto p : x) {
    ss << "<" << p.first << "," << p.second << "> ";
  }
  return ss.str();
}

#endif // ROBUST_CPP_BASIC_TYPES_H
