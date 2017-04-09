//
// Created by Christian Kroer on 3/26/17.
//

#ifndef ROBUST_CPP_ROBUST_PROGRAM_H
#define ROBUST_CPP_ROBUST_PROGRAM_H

#include "./nominal_program.h"
#include "./uncertainty_set.h"
#include <iterator>
#include <unordered_map>

class robust_program {
public:
  enum constraint_type { linear, second_order_cone };
  ~robust_program() {}
  const int dimension();
  const int num_constraints();
  const std::string model_path();
  const int num_uncertainty_sets();
  const forward_iterator robust_constraints_begin();
  const forward_iterator robust_constraints_end();
  const uncertainty_set &uncertainty_set(int id);
  const constraint_type constraint_type(int id);
  const nominal_program &get_nominal_program();
};

#endif // ROBUST_CPP_ROBUST_PROGRAM_H
