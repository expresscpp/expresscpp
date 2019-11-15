#include "expresscpp/impl/matcher.hpp"

namespace expresscpp {

bool matchLayer(std::shared_ptr<Layer> layer, std::string_view requested_path) {
  assert(layer != nullptr);
  return layer->match(requested_path);
}

}  // namespace expresscpp
