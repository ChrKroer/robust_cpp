//
// Created by Christian Kroer on 4/09/17.
//

#include "./nominal_program_dense.h"
#include "./../logging.h"
#include "gurobi_c++.h"
#include <boost/filesystem.hpp>

nominal_program_dense::nominal_program_dense(std::string model_path)
    : model_path_(model_path) {

  if (boost::filesystem::extension(model_path).compare(".mps") == 0) {
    GRBEnv env;
    env.set(GRB_IntParam_OutputFlag, 0);
    GRBModel model(env, model_path);
    dimension_ = model.get(GRB_IntAttr_NumVars);
    num_constraints_ = model.get(GRB_IntAttr_NumConstrs);
  } else {
    logger->error("unsupported nominal program file type");
  }
}
