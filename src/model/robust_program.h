//
// Created by Christian Kroer on 3/26/17.
//

#ifndef ROBUST_CPP_ROBUST_PROBLEM_H
#define ROBUST_CPP_ROBUST_PROBLEM_H

#include <unordered_map>
#include "././nominal_problem.h"

class robust_problem {
 public:
  explicit robust_problem(int dimension) : dimension_(dimension) {}
  ~robust_problem() {}

  int dimension() { return dimension_; }

 private:
  int dimension_;
  nominal_problem nominal_;
  std::unordered_map<int, uncertainty_set> uncertainty_sets_;
};

#endif //ROBUST_CPP_ROBUST_PROBLEM_H
