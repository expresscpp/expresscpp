# ExpressCpp

Fast, unopinionated, minimalist web framework for C++
Perfect for building REST APIs

[![pipeline status](https://gitlab.com/expresscpp/expresscpp/badges/master/pipeline.svg)](https://gitlab.com/expresscpp/expresscpp/commits/master)
[![expresscpp_http](https://gitlab.com/expresscpp/expresscpp/badges/master/coverage.svg?job=test:linux)](https://gitlab.com/expresscpp/expresscpp/commits/)
[![Status](https://img.shields.io/badge/quality-alpha-red)](https://img.shields.io/badge/quality-alpha-red)

## Design goals

ExpressCpp aims to be for C++ the same as express for Node.JS including its ecosystem of middlewares and extensions.

Express and Node.JS:

```js
const express = require('express')
const app = express()
const port = 3000
app.get('/', (req, res) => res.send('Hello World!'))
app.listen(port, () => console.log(`Example app listening on port ${port}!`))
```

ExpressCpp:

```cpp
auto expresscpp = ExpressCpp::GetInstance();
expresscpp->Get("/", [](auto req, auto res) { res->send("hello world!") });
const auto port = 3000;
expresscpp.listen(port, [&](){
    std::cout << "Example app listening on port "<< port << std::endl;
});
```

## Official Middlewares

* expresscpp-logger -> todo
* expresscpp-basic-auth -> todo

## Dependencies

* boost.beast
* boost.asio
* gtest (optional)
* openSSL (optional)
