//
// Created by Christian Kroer on 4/09/17.
//

#include "./../logging.h"
#include "./nominal_program_dense.h"
#include <boost/filesystem.hpp>

nominal_program_dense::nominal_program_dense(std::string model_path)
    : model_path_(model_path) {
  if (boost::filesystem::extension(model_path).compare(".mps") == 0) {
    clp_model_.readMps(model_path);
  } else {
    logger->error("unsupported nominal program file type");
  }
}
