#pragma once
#include "Color.hpp"

namespace Npi {
struct Vector2f {
  Vector2f(float u1, float v1)
  {
    u = u1;
    v = v1;
  }
  float u;
  float v;
};
struct Vector3f {
  float x;
  float y;
  float z;
};
struct ColoredVertex {
  Vector3f position;
  Npi::Color color;
};
struct TexturedVertex {
  Vector3f position;
  Vector2f textureCoords;
};

} // namespace Npi