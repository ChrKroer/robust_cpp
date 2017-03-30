//
// Created by Christian Kroer on 3/26/17.
//

#ifndef ROBUST_CPP_ROBUST_PROGRAM_H
#define ROBUST_CPP_ROBUST_PROGRAM_H

#include <unordered_map>
#include "././nominal_program.h"

class robust_program {
 public:
  explicit robust_program(int dimension) : dimension_(dimension) {}
  ~robust_program() {}

  int dimension() { return dimension_; }

 private:
  int dimension_;
  nominal_program nominal_;
  std::unordered_map<int, uncertainty_set> uncertainty_sets_;
};

#endif //ROBUST_CPP_ROBUST_PROGRAM_H
