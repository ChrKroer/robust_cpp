//
// Created by Christian Kroer on 4/22/17.
//

#ifndef ROBUST_CPP_NOMINAL_SOLVER_H
#define ROBUST_CPP_NOMINAL_SOLVER_H

#include "../basic_types.h"

class nominal_solver {
public:
  enum status { OPTIMAL, INFEASIBLE, UNDEFINED };
  virtual ~nominal_solver() {}
  virtual void optimize() = 0;
  virtual nominal_solver::status get_status() const = 0;
  virtual double get_objective() = 0;
  virtual double get_var_val(int id) = 0;
  virtual void update_constraint(int constraint_id, std::vector<std::pair<int,double>> coeffs) = 0;
};

#endif // ROBUST_CPP_NOMINAL_SOLVER_H
