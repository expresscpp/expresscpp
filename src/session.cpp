#include "expresscpp/impl/session.hpp"

#include "boost/asio.hpp"
#include "boost/asio/strand.hpp"
#include "boost/beast/http/field.hpp"
#include "expresscpp/expresscpp.hpp"

namespace expresscpp {

Session::Session(boost::asio::ip::tcp::socket &&socket, ExpressCpp *express_cpp)
    : stream_(std::move(socket)), express_cpp_(express_cpp) {
  assert(express_cpp_ != nullptr);
}

Session::~Session() {
  Console::Debug("destroying session");
}

void Session::run() {
  do_read();
}

void Session::do_read() {
  // Make the request empty before reading,
  // otherwise the operation behavior is undefined.
  req_ = {};

  // Set the timeout.
  stream_.expires_after(std::chrono::seconds(30));

  //  req_parser_ = std::make_unique<SessionParser>();
  //  req_parser_->body_limit(std::numeric_limits<std::uint64_t>::max());

  //  http::async_read(stream_, buffer_, *req_parser_,
  //                   boost::asio::bind_executor(strand_, std::bind(&Session::on_read, shared_from_this(),
  //                                                                 std::placeholders::_1, std::placeholders::_2)));

  // Read a request
  boost::beast::http::async_read(stream_, buffer_, req_,
                                 boost::beast::bind_front_handler(&Session::on_read, shared_from_this()));
}

void Session::on_read(beast::error_code ec, std::size_t bytes_transferred) {
  boost::ignore_unused(bytes_transferred);

  // This means they closed the connection
  if (ec == http::error::end_of_stream) {
    return do_close();
  }

  if (ec) {
    return fail(ec, "read");
  }

  const auto path = std::string(req_.target());
  const auto method = beastVerbToHttpMethod(req_.method());
  const auto keep_alive = req_.keep_alive();

  auto req = std::make_shared<Request>(path, method);
  auto res = std::make_shared<Response>(this);
  res->KeepAlive(keep_alive);

  for (auto const &field : req_) {
    const std::string name = boost::beast::http::to_string(field.name()).data();
    const std::string value(field.value().data(), field.value().size());
    req->setHeader(name, value);
  }

  req->setBody(req_.body());

  express_cpp_->HandleRequest(req, res, nullptr);

  if (!res->response_sent_) {
    Console::Error(fmt::format(R"(no response sent for path "{}")", req->getPath()));
  }
}

void Session::on_write(bool close, beast::error_code ec, std::size_t bytes_transferred) {
  boost::ignore_unused(bytes_transferred);

  if (ec) {
    return fail(ec, "write");
  }

  if (close) {
    // This means we should close the connection, usually because
    // the response indicated the "Connection: close" semantic.
    return do_close();
  }

  // We're done with the response so delete it
  res_ = nullptr;

  // Read another request
  do_read();
}

void Session::do_close() {
  // Send a TCP shutdown
  beast::error_code ec;
  stream_.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);

  // At this point the connection is closed gracefully
}
}  // namespace expresscpp
