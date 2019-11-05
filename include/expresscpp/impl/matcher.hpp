#pragma once

#include <memory>
#include <string_view>

#include "expresscpp/layer.hpp"

namespace expresscpp {

/**
 * Match path to a layer.
 * @param {Layer} layer
 * @param {string} path
 */
bool matchLayer(std::shared_ptr<Layer> layer, std::string_view path);

}  // namespace expresscpp
