#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "expresscpp/nextrouter.hpp"
#include "expresscpp/request.hpp"
#include "expresscpp/response.hpp"
#include "expresscpp/router.hpp"

class ExpressCpp
{
    ExpressCpp() { std::cout << "ExpressCpp created" << std::endl; }

    static std::shared_ptr<ExpressCpp> singleton;

public:
    ~ExpressCpp() { std::cout << "ExpressCpp destroyed" << std::endl; }

    static std::shared_ptr<ExpressCpp> GetInstance()
    {
        if (singleton == nullptr) {
            singleton.reset(new ExpressCpp());
        }
        return singleton;
    }

    void HandleRequest(std::shared_ptr<Request> req, std::shared_ptr<Response> res)
    {
        std::cout << "handling request" << std::endl;
        for (auto &r : routers) {
            r.HandleRequest(req, res);
        }
    }

    std::shared_ptr<Router> GetRouter()
    {
        std::cout << "getting a router" << std::endl;
        //        std::shared_ptr<Router> router;

        return std::make_shared<Router>();
    }

private:
    std::vector<Router> routers;
};
