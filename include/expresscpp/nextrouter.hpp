#pragma once

#include <iostream>

class NextRouter {
 public:
  void operator()() { std::cout << "next called" << std::endl; }
};
