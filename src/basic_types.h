#ifndef ROBUST_CPP_BASIC_TYPES_H
#define ROBUST_CPP_BASIC_TYPES_H

#include "Eigen/Core"
#include "Eigen/Sparse"
#include <iostream>
#include <vector>

#define TRACE_MSG                                                              \
  fprintf(stderr, "[%s:%d] %s here I am\n", __FILE__, __LINE__,                \
          __PRETTY_FUNCTION__); std::cout << std::endl

using vector_d = Eigen::VectorXd;
using sparse_vector_d = Eigen::SparseVector<double>;
using matrix_d = Eigen::MatrixXd;
using sparse_matrix_d = Eigen::SparseMatrix<double>;

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

inline std::string
sparse_vector_string(const sparse_vector_d &x) {
  std::stringstream ss;
  for (sparse_vector_d::InnerIterator it(x); it; ++it) {
    ss << "<" << it.index() << "," << it.value() << "> ";
  }
  return ss.str();
}

#endif // ROBUST_CPP_BASIC_TYPES_H
