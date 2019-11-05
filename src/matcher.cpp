#include "expresscpp/impl/matcher.hpp"

namespace expresscpp {

bool matchLayer(std::shared_ptr<Layer> layer, std::string_view path) {
  assert(layer != nullptr);
  return layer->match(path);
}

}  // namespace expresscpp