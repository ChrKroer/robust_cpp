//
// Created by Christian Kroer on 4/09/17.
//

#include "./nominal_program.h"
#include "./../logging.h"
#include <boost/filesystem.hpp>

nominal_program::nominal_program(std::string model_path)
    : model_path_(model_path) {
  if (boost::filesystem::extension(model_path).compare(".mps") == 0) {
    clp_model_.readMps(model_path);
  } else {
    logger->error("unsupported nominal program file type");
  }
}
