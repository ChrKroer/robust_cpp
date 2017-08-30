#include "logging.h"

std::shared_ptr<spdlog::logger> setup_logger() {
  spdlog::set_pattern("[%H:%M:%S] [%l]: %v");
  std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("console");
  return logger;
}
// std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("console");
std::shared_ptr<spdlog::logger> logger = setup_logger();
