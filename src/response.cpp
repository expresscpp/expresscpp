#include "expresscpp/response.hpp"

#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "expresscpp/console.hpp"
#include "expresscpp/impl/session.hpp"

namespace expresscpp {

Response::Response(Session* session) : session_(session) {
  // not asserting here as we want to pass nullptr to be able to use the response without beast for testing
  //  assert(session_ != nullptr);

  uuid_ = boost::uuids::random_generator()();
  Console::Debug(fmt::format("Response created \"{}\"", boostUUIDToString(uuid_)));
}

void Response::SetStatus(uint16_t status) {
  status_ = status;
  res.result(status);
}

void Response::KeepAlive(bool alive) {
  res.keep_alive(alive);
}

void Response::Send() {
  res.prepare_payload();
  SendInternal();
}

void Response::Send(std::string message) {
  res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
  res.set(http::field::content_type, "text/html");
  res.body() = std::string(message);
  res.prepare_payload();
  SendInternal();
}

void Response::Json(std::string_view json_string) {
  res.result(http::status::ok);
  res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
  res.set(http::field::content_type, "application/json");
  res.body() = std::string(json_string);
  res.prepare_payload();
  SendInternal();
}

void Response::SendInternal() {
  if (response_sent_) {
    Console::Error("ERROR: response already sent");
    return;
  }

  response_sent_ = true;

  // The lifetime of the message has to extend
  // for the duration of the async operation so
  // we use a shared_ptr to manage it.
  auto sp = std::make_shared<http::message<false, http::string_body, boost::beast::http::fields>>(std::move(res));

  if (session_ == nullptr) {
    throw std::runtime_error("session not valid");
  }

  // Store a type-erased version of the shared
  // pointer in the class to keep it alive.
  session_->res_ = sp;

  // Write the response
  http::async_write(session_->socket_, *sp,
                    std::bind(&Session::on_write, session_->shared_from_this(), sp->need_eof(), std::placeholders::_1,
                              std::placeholders::_2));
}

}  // namespace expresscpp
