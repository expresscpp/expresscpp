#include "expresscpp/fetch.hpp"

#include <cstdlib>
#include <string>

#include "boost/asio/connect.hpp"
#include "boost/asio/ip/tcp.hpp"
#include "boost/beast/core.hpp"
#include "boost/beast/http.hpp"
#include "boost/beast/version.hpp"
#include "expresscpp/console.hpp"
#include "expresscpp/url.hpp"

namespace expresscpp {

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

std::string fetch(const std::string& url, const FetchOptions options) {
  std::string string_response;

  try {
    auto parsed_url = parseURI(url);

    auto const host = parsed_url.domain;
    auto const port = parsed_url.port;
    auto required_path = parsed_url.resource;
    if (!parsed_url.query.empty()) {
      required_path += "?" + parsed_url.query;
    }
    constexpr int version = 11;

    // The io_context is required for all I/O
    net::io_context ioc;

    // These objects perform our I/O
    tcp::resolver resolver(ioc);
    tcp::socket socket{ioc};
    // Look up the domain name
    auto const results = resolver.resolve(host, port);

    // Make the connection on the IP address we get from a lookup
    boost::asio::connect(socket, results.begin(), results.end());

    // Set up an HTTP GET request message
    http::request<http::string_body> req{getBeastVerbFromExpressVerb(options.method), required_path, version};
    req.set(http::field::host, host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    for (const auto& h : options.headers) {
      req.set(h.first, h.second);
    }

    if (!options.body.empty()) {
      req.body() = options.body;
      req.prepare_payload();
    }

    // Send the HTTP request to the remote host
    http::write(socket, req);

    // This buffer is used for reading and must be persisted
    beast::flat_buffer buffer;

    // Declare a container to hold the response
    http::response<http::dynamic_body> res;

    // Receive the HTTP response
    http::read(socket, buffer, res);

    string_response = boost::beast::buffers_to_string(res.body().data());

    // Gracefully close the socket
    beast::error_code ec;
    socket.shutdown(tcp::socket::shutdown_both, ec);

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

}  // namespace expresscpp
