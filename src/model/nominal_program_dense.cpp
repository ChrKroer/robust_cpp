//
// Created by Christian Kroer on 4/09/17.
//

#include "./nominal_program_dense.h"
#include "./../basic_types.h"
#include "./../logging.h"
#include "gurobi_c++.h"
#include <boost/filesystem.hpp>

nominal_program_dense::nominal_program_dense(std::string model_path)
    : model_path_(model_path) {

  // logger->info("model_path: {}, ",
  //              boost::filesystem::path(model_path).string());
  // TRACE_MSG;
  // std::string extension = boost::filesystem::extension(model_path);
  std::string extension = model_path.substr(model_path.length() - 4);
  if (extension == ".mps") {
    GRBEnv env;
    env.set(GRB_IntParam_OutputFlag, 0);
    GRBModel model(env, model_path);
    dimension_ = model.get(GRB_IntAttr_NumVars);
    num_constraints_ = model.get(GRB_IntAttr_NumConstrs);
  } else {
    logger->error("unsupported nominal program file type");
  }
}
