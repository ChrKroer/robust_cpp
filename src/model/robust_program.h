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
  virtual int dimension() const = 0;
  virtual int num_constraints() const = 0;
  virtual int num_uncertainty_sets() const = 0;
  virtual std::unordered_set<int>::const_iterator robust_constraints_begin() const = 0;
  virtual std::unordered_set<int>::const_iterator robust_constraints_end() const = 0;
  virtual const uncertainty_set &get_uncertainty_set(int id) const = 0;
  virtual const constraint_type get_constraint_type(int id) const = 0;
};

#endif // ROBUST_CPP_ROBUST_PROGRAM_H
