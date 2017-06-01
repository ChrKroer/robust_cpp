//
// Created by Christian Kroer on 5/03/17.
//

#ifndef ROBUST_CPP_ROBUST_SOLVER_H
#define ROBUST_CPP_ROBUST_SOLVER_H

#include "../basic_types.h"

class robust_solver {
public:
  virtual ~robust_solver() = default;
  virtual vector_d current_solution() = 0;
  virtual double optimize() = 0;
  virtual int num_iterations() = 0;
  virtual nominal_solver::status get_status() = 0;
};

#endif // ROBUST_CPP_ROBUST_SOLVER_H
