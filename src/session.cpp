#include "expresscpp/impl/session.hpp"

#include "boost/asio.hpp"
#include "boost/asio/strand.hpp"
#include "boost/beast/http/field.hpp"
#include "expresscpp/console.hpp"
#include "expresscpp/expresscpp.hpp"
#include "expresscpp/impl/listener.hpp"

namespace expresscpp {

Session::Session(boost::asio::ip::tcp::socket socket, ExpressCpp *express_cpp)
    : socket_(std::move(socket)), express_cpp_(express_cpp), strand_(socket_.get_executor()) {
  assert(express_cpp_ != nullptr);
}

Session::~Session() {
  Console::Debug("destroying session");
}

void Session::run() {
  do_read();
}

void Session::do_read() {
  parser_ = std::make_unique<SessionParser>();
  parser_->body_limit(std::numeric_limits<std::uint64_t>::max());
  auto self = shared_from_this();
  http::async_read(socket_, buffer_, *parser_, [self](beast::error_code ec, std::size_t bytes_transferred) {
    self->on_read(ec, bytes_transferred);
  });
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

  auto http_request = parser_->get();

  const auto path = std::string(http_request.target());
  const auto method = beastVerbToHttpMethod(http_request.method());
  const auto keep_alive = http_request.keep_alive();

  auto req = std::make_shared<Request>(path, method);
  auto res = std::make_shared<Response>(this);
  res->KeepAlive(keep_alive);

  for (auto const &field : http_request) {
    const std::string name = boost::beast::http::to_string(field.name()).data();
    const std::string value(field.value().data(), field.value().size());
    req->setHeader(name, value);
  }

  req->setBody(http_request.body());

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
  socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
  // At this point the connection is closed gracefully
}
}  // namespace expresscpp
