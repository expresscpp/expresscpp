#pragma once

#include <filesystem>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

#include "boost/asio.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "expresscpp/handler.hpp"
#include "expresscpp/impl/listener.hpp"
#include "expresscpp/impl/routing_stack.hpp"
#include "expresscpp/nextrouter.hpp"
#include "expresscpp/request.hpp"
#include "expresscpp/response.hpp"
#include "expresscpp/route.hpp"
#include "expresscpp/router.hpp"
#include "expresscpp/types.hpp"

#ifdef EXPRESSCPP_ENABLE_STATIC_FILE_PROVIDER
#include "expresscpp/middleware/staticfileprovider.hpp"
#endif
namespace expresscpp {

class ExpressCpp {
  friend class Session;

 public:
  ExpressCpp();

  ExpressCpp(uint8_t number_to_threads);
  ~ExpressCpp();

  // TODO(gocarlos): convert handler to parameter pack
  void Get(std::string_view path, express_handler_wn_t handler);
  void Post(std::string_view path, express_handler_wn_t handler);
  void Delete(std::string_view path, express_handler_wn_t handler);
  void Patch(std::string_view path, express_handler_wn_t handler);

  auto GetBaseRouter();

  std::shared_ptr<Route> CreateRoute(const std::string_view);

  void Use(express_handler_t handler);

  /*!
   * Proxy `Router#Use()` to add middleware to the app router.
   * See Router#use() documentation for details.
   *
   * If the _fn_ parameter is an express app, then it will be
   * mounted at the _route_ specified.
   */
  void Use(express_handler_wn_t handler);
  void Use(std::string_view path, express_handler_t handler);
  void Use(std::string_view path, express_handler_wn_t handler);
  void Use(std::string_view path, RouterPtr router);
#ifdef EXPRESSCPP_ENABLE_STATIC_FILE_PROVIDER
  void Use(StaticFileProviderPtr static_file_provider);
  void Use(std::string_view path, StaticFileProviderPtr static_file_provider);
#endif

  //! called to start listening on port @ref port_
  ExpressCpp& Listen(uint16_t port, ready_fn_cb_error_code_t callback);

  //! @brief blocks until CTRL+C
  void Block();
  void InstallSignalHandler();
  static void HandleSignal(int signal);

  RouterPtr GetRouter();
  RouterPtr GetRouter(std::string_view name);

#ifdef EXPRESSCPP_ENABLE_STATIC_FILE_PROVIDER
  StaticFileProviderPtr GetStaticFileProvider(const std::filesystem::path& path_to_root_folder);
#endif

  std::string DumpRoutingTable() const;
  std::vector<RoutingStack> Stack() const;

  std::uint16_t port() const;
  void setPort(const std::uint16_t& port);

  /**
   * Dispatch a req, res pair into the application. Starts pipeline processing.
   */
  void HandleRequest(express_request_t req, express_response_t res, std::function<void()> callback);
 private:
  void RegisterPath(const std::string_view path, const HttpMethod method, express_handler_t handler,
                    const bool is_router = false);
  void RegisterPath(const std::string_view path, const HttpMethod method, express_handler_wn_t handler);
  void Init();

  void lazyrouter();

  std::shared_ptr<Router> _router;

  //  std::vector<std::pair<std::string_view, std::shared_ptr<Router>>> routers_;

#ifdef EXPRESSCPP_ENABLE_STATIC_FILE_PROVIDER
  std::vector<StaticFileProviderPtr> static_file_providers_;
#endif

  std::shared_ptr<Listener> listener_{nullptr};

  std::map<std::string_view, express_handler_vector_t> handler_map_;

  std::vector<Route> routes_;

  std::size_t threads_{4u};

  std::uint16_t port_;

  bool listening_{false};

  //! @brief the io_context is required for all I/O
  boost::asio::io_context ioc;
  std::vector<std::thread> io_threads;
};

}  // namespace expresscpp
