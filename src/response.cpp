#include "expresscpp/response.hpp"

#include "expresscpp/impl/session.hpp"

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
    std::cerr << "ERROR: response already sent, " << std::endl;
    return;
  }

  response_sent_ = true;

  // The lifetime of the message has to extend
  // for the duration of the async operation so
  // we use a shared_ptr to manage it.
  auto sp = std::make_shared<http::message<false, http::string_body, boost::beast::http::fields>>(std::move(res));

  // Store a type-erased version of the shared
  // pointer in the class to keep it alive.
  session_->res_ = sp;

  // Write the response
  http::async_write(session_->stream_, *sp,
                    beast::bind_front_handler(&Session::on_write, session_->shared_from_this(), sp->need_eof()));
}
