#include "expresscpp/impl/matcher.hpp"

namespace expresscpp {

bool matchLayer(std::shared_ptr<Layer> layer, std::string_view requested_path) {
  assert(layer != nullptr);
  // TODO(marco): implement this

  if (layer->getRoute() == nullptr) {
    // there is no route left -> this layer is a middleware
    return true;
  }

  if (requested_path == layer->path_) {
    return true;
  }

  return false;
  //  return layer->match(path);
}

}  // namespace expresscpp
