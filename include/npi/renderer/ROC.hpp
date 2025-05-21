#pragma once
#include <npi/renderer/RenderContext.hpp>

namespace Npi {

class ROC {
 public:
  virtual void draw(Npi::RenderContext t_context) = 0;
};
}  // namespace Npi