#pragma once
#include "Color.hpp"
#include <citro3d.h>


namespace Npi {

class Material {
public:
  Material();
  Material(int kar, int kag, int kab, int kdr, int kdg, int kdb, int ksr,
           int ksg, int ksb);
  Material(C3D_Material t_material);
  Material(const Npi::Material &t_source);
  virtual ~Material();
  void setMaterial(C3D_Material t_material);
  C3D_Material *getMaterial() const;
  void setAmbient(Npi::Color t_color);
  void setAmbient(int t_r, int t_g, int t_b);
  Npi::Color getAmbient();
  void setDiffuse(Npi::Color t_color);
  void setDiffuse(int t_r, int t_g, int t_b);
  Npi::Color getDiffuse();
  void setSpecular0(Npi::Color t_color);
  void setSpecular0(int t_r, int t_g, int t_b);
  Npi::Color getSpecular0();
  void setSpecular1(Npi::Color t_color);
  void setSpecular1(int t_r, int t_g, int t_b);
  Npi::Color getSpecular1();
  void setEmission(Npi::Color t_color);
  void setEmission(int t_r, int t_g, int t_b);
  Npi::Color getEmission();
  Npi::Material &operator=(const Npi::Material &rhs);

private:
  void updateMaterial();

  /* data */
  Npi::Color m_ambient, m_diffuse, m_specular0, m_specular1, m_emission;
  C3D_Material *m_material;
};
} // namespace Npi