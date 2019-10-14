#pragma once

#include <cstdint>
#include <iostream>

class Response
{
public:
    void SetStatus(uint8_t new_status) { status = new_status; }
    void Send(std::string message) { std::cout << "sending: " << message << std::endl; }

   private:
    uint8_t status;
};
