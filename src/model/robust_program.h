//
// Created by Christian Kroer on 3/26/17.
//

#ifndef ROBUST_CPP_ROBUST_PROGRAM_H
#define ROBUST_CPP_ROBUST_PROGRAM_H

#include "./nominal_program.h"
#include "./uncertainty_set.h"
#include <iterator>
#include <unordered_set>

class robust_program {
public:
  enum constraint_type { linear, second_order_cone };
  ~robust_program() {}
  int dimension() const;
  int num_constraints() const;
  std::string model_path() const;
  int num_uncertainty_sets() const;
  std::unordered_set<int>::const_iterator robust_constraints_begin() const;
  std::unordered_set<int>::const_iterator robust_constraints_end() const;
  const uncertainty_set &get_uncertainty_set(int id) const;
  const constraint_type get_constraint_type(int id) const;
  const nominal_program &get_nominal_program() const;
};

#endif // ROBUST_CPP_ROBUST_PROGRAM_H
