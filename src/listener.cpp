#include "expresscpp/impl/listener.hpp"
namespace expresscpp {

Listener::Listener(const std::string& address, const uint16_t port, ExpressCpp* express_cpp,
                   ready_fn_cb_error_code_t error_callback)
    : acceptor_(net::make_strand(ioc_)), express_cpp_(express_cpp), io_threads(threads_) {
  assert(express_cpp_ != nullptr);
  const auto ip_address = boost::asio::ip::make_address(address);
  auto asio_endpoint = boost::asio::ip::tcp::endpoint{ip_address, port};
  Init(asio_endpoint, error_callback);
}

Listener::~Listener() {
  Console::Debug("destroying listener");
  Stop();
}

void Listener::Init(boost::asio::ip::tcp::endpoint endpoint, ready_fn_cb_error_code_t error_callback) {
  boost::beast::error_code ec;

  // Open the acceptor
  acceptor_.open(endpoint.protocol(), ec);
  if (ec) {
    error_callback(ec);
    fail(ec, "open");
    return;
  }

  // Allow address reuse
  acceptor_.set_option(net::socket_base::reuse_address(true), ec);
  if (ec) {
    error_callback(ec);
    fail(ec, "set_option");
    return;
  }

  // Bind to the server address
  acceptor_.bind(endpoint, ec);
  if (ec) {
    error_callback(ec);
    fail(ec, "bind");
    return;
  }

  // Start listening for connections
  acceptor_.listen(net::socket_base::max_listen_connections, ec);
  if (ec) {
    error_callback(ec);
    fail(ec, "listen");
    return;
  }
}

void Listener::run() {
  do_accept();
}

void Listener::launch_threads() {
  // Run the I/O service on the requested number of threads
  io_threads.reserve(threads_);
  for (auto i = threads_; i > 0; --i) {
    io_threads.emplace_back([this] { ioc_.run(); });
  }
  listening_ = true;
}

void Listener::Stop() {
  if (!listening_) {
    return;
  }
  ioc_.stop();
  acceptor_.close();
  for (auto& t : io_threads) {
    if (t.joinable()) {
      t.join();
    }
  }
  Console::Debug("stopping listener");
  listening_ = false;
}

void Listener::do_accept() {
  // The new connection gets its own strand
  acceptor_.async_accept(net::make_strand(ioc_), beast::bind_front_handler(&Listener::on_accept, this));
}

void Listener::on_accept(beast::error_code ec, tcp::socket socket) {
  if (ec) {
    fail(ec, "accept");
  } else {
    // Create the session and run it
    std::make_shared<Session>(std::move(socket), express_cpp_)->run();
  }

  // Accept another connection
  do_accept();
}

}  // namespace expresscpp
