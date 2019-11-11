#pragma once

#include <condition_variable>
#include <map>
#include <mutex>
#include <string>

#include "boost/beast/http/verb.hpp"

class TestCallSleeper {
 public:
  TestCallSleeper(size_t expected_call_count) {
    std::unique_lock<std::mutex> lock(mutex_);
    expected_call_count_ = expected_call_count;
  }

  bool Wait(std::chrono::seconds wait_time_in_sec = std::chrono::seconds(5)) {
    std::unique_lock<std::mutex> lock(mutex_);
    while (call_counter_ != expected_call_count_) {
      return condition_.wait_for(lock, std::chrono::seconds(wait_time_in_sec)) == std::cv_status::no_timeout;
    }
    return true;
  }

  void Call() {
    std::unique_lock<std::mutex> lock(mutex_);
    ++call_counter_;
    condition_.notify_all();
  }

 private:
  size_t call_counter_{0};
  size_t expected_call_count_{0};
  std::mutex mutex_;
  std::condition_variable condition_;
};
