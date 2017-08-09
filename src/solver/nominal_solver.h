//
// Created by Christian Kroer on 4/22/17.
//

#ifndef ROBUST_CPP_NOMINAL_SOLVER_H
#define ROBUST_CPP_NOMINAL_SOLVER_H

#include "../basic_types.h"
#include "./../model/uncertainty_constraint.h"
#include <string>

class nominal_solver {
public:
  enum status { OPTIMAL, INFEASIBLE, UNDEFINED };
  virtual ~nominal_solver() = default;
  virtual void optimize() = 0;
  virtual nominal_solver::status get_status() const = 0;
  virtual double get_objective() = 0;
  virtual double get_var_val(const int id) = 0;
  virtual void add_constraint(const vector_d &coeffs,
                              const uncertainty_constraint &unc) = 0;
  virtual void update_constraint(const int constraint_id, const vector_d &coeffs,
                                 const uncertainty_constraint &unc) = 0;
  virtual double get_variable_value(const std::string var) = 0;
};

#endif // ROBUST_CPP_NOMINAL_SOLVER_H
