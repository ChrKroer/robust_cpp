//
// Created by Christian Kroer on 3/26/17.
//

#ifndef ROBUST_CPP_UNCERTAINTY_CONSTRAINT_H
#define ROBUST_CPP_UNCERTAINTY_CONSTRAINT_H

#include <string>
#include <utility>
#include <vector>
#include "./../basic_types.h"
#include "./../domain/domain.h"

class uncertainty_constraint {
 public:
  // enum constraint_type { EUCLIDEAN_BALL, SIMPLEX, POINT };
  enum function_type { LINEAR, QUADRATIC };
  virtual ~uncertainty_constraint() = default;
  virtual function_type get_function_type() const = 0;
  virtual int dimension() const = 0;
  // returns the parameter instantiation that maximizes the constraint function,
  // as well as the constraint value.
  virtual std::pair<double, vector_d> maximizer(
      const vector_d current) const = 0;
  /**
   * @brief Provides the gradient of the uncertainty constraints LHS wrt.
   * unc_vec.
   *
   * `solution` should be a potential, not necessarily feasible, solution to the
   * model that the constraint belongs to, and unc_vec should be a vector from
   * the uncertainty set at this constraint.
   */
  virtual vector_d gradient(const vector_d &solution,
                            const vector_d &unc_vec) const = 0;
  virtual const domain *get_domain() const = 0;
  virtual double violation_amount(const vector_d &solution,
                                  const vector_d &constraint_params) const = 0;
  std::pair<const std::vector<double> , const std::vector<std::string> >
  get_certain_var() const {
    const std::vector<double> cvc = certain_variable_coefficient_;
    const std::vector<std::string> cvn = certain_variable_name_;
    return std::make_pair(cvc, cvn);
  };

  const std::string &get_constraint_name() const { return name_; };

 protected:
  std::vector<double> certain_variable_coefficient_;
  std::vector<std::string> certain_variable_name_;
  std::vector<int> certain_variable_index_;
  std::string name_;
};

#endif  // ROBUST_CPP_UNCERTAINTY_CONSTRAINT_H
