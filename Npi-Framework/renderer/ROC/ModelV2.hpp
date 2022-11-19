#pragma once
#include "Mesh.hpp"
// #include "ObjMateial.hpp"

namespace Npi {

class ModelV2 : public Npi::ROC {
public:
  bool LoadFromFile(std::string path);
  void draw(Npi::RenderContext t_context);
  void rotateYaw(float t_delta, bool t_radians = false);
  void setYaw(float t_rotation, bool t_radians = false);
  void setScale(float t_scaleX, float t_scaleY, float t_scaleZ);
  
private:
  void update();

  std::vector<Npi::Mesh *> mesches;
};
} // namespace Npi