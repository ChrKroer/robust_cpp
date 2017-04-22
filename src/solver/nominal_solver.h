//
// Created by Christian Kroer on 4/22/17.
//

#ifndef ROBUST_CPP_NOMINAL_SOLVER_H
#define ROBUST_CPP_NOMINAL_SOLVER_H

#include "../basic_types.h"

class nominal_solver {
public:
  virtual ~nominal_solver() {}
  virtual void optimize() = 0;
  virtual int get_status() = 0;
  virtual int get_objective() = 0;
  virtual int get_var_val(int id) = 0;
  virtual void update_constraint(int constraint_id, std::vector<int> var_ids,
                                 vector_d coeffs);
  virtual void add_constraint(std::vector<int> var_ids, vector_d coeffs);
};

#endif // ROBUST_CPP_NOMINAL_SOLVER_H
