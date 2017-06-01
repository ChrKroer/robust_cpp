//
// Created by Christian Kroer on 4/11/17.
//

#ifndef ROBUST_CPP_ROBUST_PROGRAM_DENSE_H
#define ROBUST_CPP_ROBUST_PROGRAM_DENSE_H

#include "./robust_program.h"
#include "./nominal_program.h"
#include "./uncertainty_constraint.h"
#include <iterator>
#include <unordered_set>

class robust_program_dense : public robust_program {
public:
  virtual ~robust_program_dense() = default;
  virtual int dimension() const = 0;
  virtual int num_constraints() const = 0;
  virtual int num_uncertainty_constraints() const = 0;
  virtual std::unordered_set<int>::const_iterator robust_constraints_begin() const = 0;
  virtual std::unordered_set<int>::const_iterator robust_constraints_end() const = 0;
  virtual const uncertainty_constraint &get_uncertainty_constraint(int id) const = 0;
  virtual const nominal_program &get_nominal_program() const = 0;
  virtual std::string nominal_model_path() const = 0;
};

#endif // ROBUST_CPP_ROBUST_PROGRAM_DENSE_H
