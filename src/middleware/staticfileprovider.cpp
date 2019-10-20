#include "expresscpp/middleware/staticfileprovider.hpp"

#include "expresscpp/impl/session.hpp"
#include "expresscpp/impl/utils.hpp"

StaticFileProvider::StaticFileProvider(std::filesystem::__cxx11::path path_to_root_folder)
    : path_to_root_folder_(path_to_root_folder) {
  std::cout << "created static file provider for path " << path_to_root_folder_ << std::endl;
}

void StaticFileProvider::UsePrefix(std::string_view path) { doc_root = path; }

void StaticFileProvider::HandleRequests(express_request_t req, express_response_t res) {
  std::cout << "handle file response" << std::endl;

  auto requested_path = path_to_root_folder_ / std::filesystem::path(req->path_);

  if (std::filesystem::exists(requested_path)) {
    std::cout << "file: " << requested_path << " exists" << std::endl;
  } else {
    std::cout << "file: " << requested_path << " does not exists" << std::endl;
  }

  //  std::string index_html_content =
  //      R"(<!doctype html>
  //      <html>af
  //      <head>
  //      <title>This is the title of the webpage!</title>
  //      </head>
  //      <body>
  //      <p>This is a paragraph.</p>
  //      </body>
  //      </html>)";

  // Request path must be absolute and not contain "..".
  if (req->path_.empty() || req->path_[0] != '/' ||
      req->path_.find("..") != std::string_view::npos) {
    res->SetStatus(static_cast<uint16_t>(boost::beast::http::status::bad_request));
    res->Send("Illegal request-target");
    return;
  }

  // Build the path to the requested file
  std::string path = path_cat(boost::beast::string_view(doc_root.data()),
                              boost::beast::string_view(req->path_.data()));

  if (req->path_.back() == '/') {
    path.append("index.html");
  }

  std::cout << "accessing file: " << path << std::endl;

  // Attempt to open the file
  beast::error_code ec;
  http::file_body::value_type body;
  body.open(path.c_str(), beast::file_mode::scan, ec);

  // Handle the case where the file doesn't exist
  if (ec == beast::errc::no_such_file_or_directory) {
    res->SetStatus(static_cast<uint16_t>(boost::beast::http::status::not_found));
    res->Send("not found");
    return;
  }

  // Handle an unknown error
  if (ec) {
    res->SetStatus(static_cast<uint16_t>(boost::beast::http::status::internal_server_error));
    res->Send(ec.message());
    return;
  }

  // Cache the size since we need it after the move
  auto const size = body.size();

  // Respond to HEAD request
  if (req->method_ == HttpMethod::Head) {
    res->res.set(http::field::content_type, mime_type(path));
    res->res.content_length(size);
    res->SendInternal();
    return;
  }

  // Respond to GET request
  http::response<http::file_body> beast_res{std::piecewise_construct,
                                            std::make_tuple(std::move(body)),
                                            std::make_tuple(http::status::ok, 11)};
  beast_res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
  beast_res.set(http::field::content_type, mime_type(path));
  beast_res.content_length(size);
  //  beast_res.keep_alive(req.keep_alive());
  //  res->res = beast_res

  if (res->response_sent_) {
    std::cerr << "ERROR: response already sent, " << std::endl;
    return;
  }

  res->response_sent_ = true;

  // The lifetime of the message has to extend
  // for the duration of the async operation so
  // we use a shared_ptr to manage it.
  auto sp = std::make_shared<
      boost::beast::http::message<false, http::file_body, boost::beast::http::fields>>(
      std::move(beast_res));

  // Store a type-erased version of the shared
  // pointer in the class to keep it alive.
  res->session_->res_ = sp;

  // Write the response
  boost::beast::http::async_write(
      res->session_->stream_, *sp,
      boost::beast::bind_front_handler(&Session::on_write, res->session_->shared_from_this(),
                                       sp->need_eof()));
}
