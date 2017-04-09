//
// Created by Christian Kroer on 3/26/17.
//

#ifndef ROBUST_CPP_ROBUST_PROGRAM_H
#define ROBUST_CPP_ROBUST_PROGRAM_H

#include "./nominal_program.h"
#include "./uncertainty_set.h"
#include <unordered_map>

class robust_program {
public:
  enum constraint_type { linear, second_order_cone };
  ~robust_program() {}
  const int dimension();
  const int num_constraints();
  const std::string model_path();
  const int num_uncertainty_sets();
  const uncertainty_set &uncertainty_set(int id);
  const constraint_type constraint_type(int id);
};

#endif // ROBUST_CPP_ROBUST_PROGRAM_H
