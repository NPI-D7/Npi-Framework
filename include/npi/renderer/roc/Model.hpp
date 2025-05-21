#pragma once
#include <npi/renderer/3d/Mesh.hpp>

namespace Npi {

class Model : public Npi::ROC {
 public:
  Model() = default;
  Model(const std::string& path) { LoadFromFile(path); }
  bool LoadFromFile(const std::string& path);
  void draw(Npi::RenderContext t_context);
  void rotateYaw(float t_delta, bool t_radians = false);
  void setYaw(float t_rotation, bool t_radians = false);
  void setScale(float t_scaleX, float t_scaleY, float t_scaleZ);

 private:
  void update();

  std::vector<Npi::Mesh*> mesches;
};
}  // namespace Npi