#include <iostream>
#include <map>
#include <memory>
#include <vector>

#include "expresscpp/expresscpp.hpp"

void exampleHandler(std::shared_ptr<Request> req, std::shared_ptr<Response> res, NextRouter next)
{
    std::cout << "exampleHandler called " << std::endl;
    if (req->headers.empty()) {
        next();
    }

    res->SetStatus(200);
}

int main()
{
    std::cout << "Hello World!" << std::endl;

    auto expresscpp = ExpressCpp::GetInstance();
    auto router = expresscpp->GetRouter();
    router->Use("/api/v0/healthstatus", exampleHandler);
    router->Post("/api/v0/login", []() { std::cout << "lambda handler called " << std::endl; });
    return 0;
}
