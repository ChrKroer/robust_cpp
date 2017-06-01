//
// Created by Christian Kroer on 3/26/17.
//

#ifndef ROBUST_CPP_ROBUST_PROGRAM_H
#define ROBUST_CPP_ROBUST_PROGRAM_H

#include "./nominal_program.h"
#include "./uncertainty_constraint.h"
#include <iterator>
#include <unordered_set>

class robust_program {
public:
  virtual ~robust_program() = default;
  virtual int dimension() const = 0;
  virtual int num_constraints() const = 0;
  virtual int num_uncertainty_constraints() const = 0;
  virtual std::unordered_set<int>::const_iterator robust_constraints_begin() const = 0;
  virtual std::unordered_set<int>::const_iterator robust_constraints_end() const = 0;
  virtual const uncertainty_constraint &get_uncertainty_constraint(int id) const = 0;
};

#endif // ROBUST_CPP_ROBUST_PROGRAM_H
