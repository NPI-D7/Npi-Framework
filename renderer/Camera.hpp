#pragma once
#include "Vertex.hpp"
#include <citro3d.h>


namespace Npi {
class Camera {
public:
  Camera();
  void setPitch(float t_rotation, bool t_radians = false);
  float getPitch(bool t_radians = false);
  void setYaw(float t_rotation, bool t_radians = false);
  float getYaw(bool t_radians = false);
  void setRoll(float t_rotation, bool t_radians = false);
  float getRoll(bool t_radians = false);
  void setRotation(float t_pitch, float t_yaw, float t_roll,
                   bool t_radians = false);
  void rotatePitch(float t_delta, bool t_radians = false);
  void rotateYaw(float t_delta, bool t_radians = false);
  void rotateRoll(float t_delta, bool t_radians = false);
  void setPositionX(float t_position);
  float getPositionX();
  void setPositionY(float t_position);
  float getPositionY();
  void setPositionZ(float t_position);
  float getPositionZ();
  void setPosition(float t_positionX, float t_positionY, float t_positionZ);
  void setPosition(Npi::Vector3f t_position);
  void moveX(float t_delta);
  void moveY(float t_delta);
  void moveZ(float t_delta);
  C3D_Mtx &getViewMatrix();

private:
  /* data */
  float m_posX, m_posY, m_posZ, m_rotationX, m_rotationY, m_rotationZ;
  C3D_Mtx m_view;
};
} // namespace Npi