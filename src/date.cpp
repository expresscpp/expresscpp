#include "expresscpp/date.hpp"

#include <chrono>
#include <iomanip>

namespace expresscpp {

std::string Date::getTime() {
  const auto timestamp_ = std::chrono::system_clock::now();
  const time_t now_time = std::chrono::system_clock::to_time_t(timestamp_);
  const auto gmt_time = gmtime(&now_time);
  std::stringstream ss;
  ss << std::put_time(gmt_time, "%Y-%m-%d %H:%M:%S");
  return ss.str();
}

}  // namespace expresscpp
