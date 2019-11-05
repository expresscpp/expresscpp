#include "test_utils.hpp"

#include <cstdlib>
#include <iostream>
#include <string>

#include "boost/asio/connect.hpp"
#include "boost/asio/ip/tcp.hpp"
#include "boost/beast/core.hpp"
#include "boost/beast/http.hpp"
#include "boost/beast/version.hpp"
#include "expresscpp/console.hpp"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

using namespace expresscpp;

std::string getResponse(const std::string& path, boost::beast::http::verb verb,
                        std::map<std::string, std::string> headers) {
  std::string string_response;

  try {
    auto const host = "localhost";
    auto const port = "8081";
    //    auto const target = "/";
    int version = 11;

    // The io_context is required for all I/O
    net::io_context ioc;

    // These objects perform our I/O
    tcp::resolver resolver(ioc);
    beast::tcp_stream stream(ioc);

    // Look up the domain name
    auto const results = resolver.resolve(host, port);

    // Make the connection on the IP address we get from a lookup
    stream.connect(results);

    // Set up an HTTP GET request message
    http::request<http::string_body> req{verb, path, version};
    req.set(http::field::host, host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    for (const auto& h : headers) {
      req.set(h.first, h.second);
    }

    // Send the HTTP request to the remote host
    http::write(stream, req);

    // This buffer is used for reading and must be persisted
    beast::flat_buffer buffer;

    // Declare a container to hold the response
    http::response<http::dynamic_body> res;

    // Receive the HTTP response
    http::read(stream, buffer, res);

    // Write the message to standard out
    //    std::cout << res << std::endl;

    string_response = boost::beast::buffers_to_string(res.body().data());

    // Gracefully close the socket
    beast::error_code ec;
    stream.socket().shutdown(tcp::socket::shutdown_both, ec);

    // not_connected happens sometimes so don't bother reporting it.
    if (ec && ec != beast::errc::not_connected) {
      throw beast::system_error{ec};
    }
    // If we get here then the connection is closed gracefully
  } catch (std::exception const& e) {
    Console::Error(fmt::format("Error: {}", e.what()));
  }
  return string_response;
}
