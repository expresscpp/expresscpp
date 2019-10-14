#include "expresscpp/response.hpp"

#include "expresscpp/impl/session.hpp"

void Response::Send(std::string message) {
  std::cout << "sending: " << message << std::endl;
  //      // Returns a bad request response
  //      auto const bad_request = [&req](beast::string_view why) {
  //        http::response<http::string_body> res{http::status::bad_request,
  //                                              req.version()};
  //        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
  //        res.set(http::field::content_type, "text/html");
  //        res.keep_alive(req.keep_alive());
  //        res.body() = std::string(why);
  //        res.prepare_payload();
  //        return res;
  //      };

  //      // Returns a not found response
  //      auto const not_found = [&req](beast::string_view target) {
  //        http::response<http::string_body> res{http::status::not_found,
  //                                              req.version()};
  //        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
  //        res.set(http::field::content_type, "text/html");
  //        res.keep_alive(req.keep_alive());
  //        res.body() =
  //            "The resource '" + std::string(target) + "' was not found.";
  //        res.prepare_payload();
  //        return res;
  //      };

  //      // Returns a server error response
  //      auto const server_error = [&req](beast::string_view what) {
  //        http::response<http::string_body> res{
  //            http::status::internal_server_error, req.version()};
  //        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
  //        res.set(http::field::content_type, "text/html");
  //        res.keep_alive(req.keep_alive());
  //        res.body() = "An error occurred: '" + std::string(what) + "'";
  //        res.prepare_payload();
  //        return res;
  //      };

  http::response<http::string_body> res{http::status::ok, 11};
  res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
  res.set(http::field::content_type, "text/html");
  //    res.keep_alive(req.keep_alive());
  res.body() = std::string(message);
  res.prepare_payload();
  SendInternal(res);
}

void Response::Json(std::string_view json_string) {
  http::response<http::string_body> res{http::status::ok, 11};
  res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
  res.set(http::field::content_type, "application/json");
  //    res.keep_alive(req.keep_alive());
  res.body() = std::string(json_string);
  res.prepare_payload();
  SendInternal(res);
}

void Response::SendInternal(http::response<http::string_body> res) {
  // The lifetime of the message has to extend
  // for the duration of the async operation so
  // we use a shared_ptr to manage it.
  auto sp = std::make_shared<
      http::message<false, http::string_body, boost::beast::http::fields>>(
      std::move(res));

  // Store a type-erased version of the shared
  // pointer in the class to keep it alive.
  session_->res_ = sp;

  // Write the response
  http::async_write(
      session_->stream_, *sp,
      beast::bind_front_handler(&session::on_write,
                                session_->shared_from_this(), sp->need_eof()));
}
