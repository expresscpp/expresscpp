#pragma once

#include <map>
#include <string>

class Request
{
public:
    std::map<std::string, std::string> headers;
};
