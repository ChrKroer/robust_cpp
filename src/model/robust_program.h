//
// Created by Christian Kroer on 3/26/17.
//

#ifndef ROBUST_CPP_ROBUST_PROGRAM_H
#define ROBUST_CPP_ROBUST_PROGRAM_H

#include <unordered_map>
#include "././nominal_program.h"
#include "././uncertainty_set.h"

class robust_program {
 public:
  explicit robust_program(int dimension, nominal_program nominal)
      : dimension_(dimension),
        nominal_(nominal)
  {}
  ~robust_program() {}

  void add_uncertainty_set(int constraint_id, uncertainty_set u) {
    uncertainty_sets_[constraint_id] = u;
  }
  int dimension() { return dimension_; }

 private:
  int dimension_;
  nominal_program nominal_;
  std::unordered_map<int, uncertainty_set> uncertainty_sets_;
};

#endif //ROBUST_CPP_ROBUST_PROGRAM_H
