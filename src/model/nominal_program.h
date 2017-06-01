//
// Created by Christian Kroer on 3/26/17.
//

#ifndef ROBUST_CPP_NOMINAL_PROGRAM_H
#define ROBUST_CPP_NOMINAL_PROGRAM_H

class nominal_program {
public:
  virtual ~nominal_program() = default;
  virtual int dimension() = 0;
  virtual int num_constraints() = 0;
};

#endif // ROBUST_CPP_NOMINAL_PROGRAM_H
