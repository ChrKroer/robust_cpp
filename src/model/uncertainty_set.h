//
// Created by Christian Kroer on 3/26/17.
//

#ifndef ROBUST_CPP_UNCERTAINTY_SET_H
#define ROBUST_CPP_UNCERTAINTY_SET_H

class uncertainty_set {
public:
  explicit uncertainty_set(int dimension) : dimension_(dimension) {}
  ~uncertainty_set() {}

  int dimension() { return dimension_; }

private:
  int dimension_;
};

#endif // ROBUST_CPP_UNCERTAINTY_SET_H
