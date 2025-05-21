#include "Cube.hpp"

namespace Npi {
Cube::Cube(float t_length, float t_width, float t_height) {
  m_length = t_length;
  m_width = t_width;
  m_height = t_height;
  update();
}

void Cube::setLength(float t_length) { m_length = t_length; }

float Cube::getLength() { return m_length; }

void Cube::setWidth(float t_width) { m_width = t_width; }

float Cube::getWidth() { return m_width; }

void Cube::setHeight(float t_height) { m_height = t_height; }

float Cube::getHeight() { return m_height; }

// private methods
void Cube::update() {
  clearVertices();

  // first face (PZ)
  addPolygon(Npi::Polygon({{-(m_width / 2), -(m_height / 2), (m_length / 2)},
                           {0.0f, 0.0f},
                           {0.0f, 0.0f, +1.0f}},
                          {{(m_width / 2), -(m_height / 2), (m_length / 2)},
                           {1.0f, 0.0f},
                           {0.0f, 0.0f, +1.0f}},
                          {{(m_width / 2), (m_height / 2), (m_length / 2)},
                           {1.0f, 1.0f},
                           {0.0f, 0.0f, +1.0f}}));

  addPolygon(Npi::Polygon({{(m_width / 2), (m_height / 2), (m_length / 2)},
                           {1.0f, 1.0f},
                           {0.0f, 0.0f, +1.0f}},
                          {{-(m_width / 2), (m_height / 2), (m_length / 2)},
                           {0.0f, 1.0f},
                           {0.0f, 0.0f, +1.0f}},
                          {{-(m_width / 2), -(m_height / 2), (m_length / 2)},
                           {0.0f, 0.0f},
                           {0.0f, 0.0f, +1.0f}}));

  // second face (MZ)
  addPolygon(Npi::Polygon({{-(m_width / 2), -(m_height / 2), -(m_length / 2)},
                           {0.0f, 0.0f},
                           {0.0f, 0.0f, -1.0f}},
                          {{-(m_width / 2), (m_height / 2), -(m_length / 2)},
                           {1.0f, 0.0f},
                           {0.0f, 0.0f, -1.0f}},
                          {{(m_width / 2), (m_height / 2), -(m_length / 2)},
                           {1.0f, 1.0f},
                           {0.0f, 0.0f, -1.0f}}));

  addPolygon(Npi::Polygon({{(m_width / 2), (m_height / 2), -(m_length / 2)},
                           {1.0f, 1.0f},
                           {0.0f, 0.0f, -1.0f}},
                          {{(m_width / 2), -(m_height / 2), -(m_length / 2)},
                           {0.0f, 1.0f},
                           {0.0f, 0.0f, -1.0f}},
                          {{-(m_width / 2), -(m_height / 2), -(m_length / 2)},
                           {0.0f, 0.0f},
                           {0.0f, 0.0f, -1.0f}}));

  // third face (PX)
  addPolygon(Npi::Polygon({{(m_width / 2), -(m_height / 2), -(m_length / 2)},
                           {0.0f, 0.0f},
                           {+1.0f, 0.0f, 0.0f}},
                          {{(m_width / 2), (m_height / 2), -(m_length / 2)},
                           {1.0f, 0.0f},
                           {+1.0f, 0.0f, 0.0f}},
                          {{(m_width / 2), (m_height / 2), (m_length / 2)},
                           {1.0f, 1.0f},
                           {+1.0f, 0.0f, 0.0f}}));

  addPolygon(Npi::Polygon({{(m_width / 2), (m_height / 2), (m_length / 2)},
                           {1.0f, 1.0f},
                           {+1.0f, 0.0f, 0.0f}},
                          {{(m_width / 2), -(m_height / 2), (m_length / 2)},
                           {0.0f, 1.0f},
                           {+1.0f, 0.0f, 0.0f}},
                          {{(m_width / 2), -(m_height / 2), -(m_length / 2)},
                           {0.0f, 0.0f},
                           {+1.0f, 0.0f, 0.0f}}));

  // fourth face (MX)
  addPolygon(Npi::Polygon({{-(m_width / 2), -(m_height / 2), -(m_length / 2)},
                           {0.0f, 0.0f},
                           {-1.0f, 0.0f, 0.0f}},
                          {{-(m_width / 2), -(m_height / 2), (m_length / 2)},
                           {1.0f, 0.0f},
                           {-1.0f, 0.0f, 0.0f}},
                          {{-(m_width / 2), (m_height / 2), (m_length / 2)},
                           {1.0f, 1.0f},
                           {-1.0f, 0.0f, 0.0f}}));

  addPolygon(Npi::Polygon({{-(m_width / 2), (m_height / 2), (m_length / 2)},
                           {1.0f, 1.0f},
                           {-1.0f, 0.0f, 0.0f}},
                          {{-(m_width / 2), (m_height / 2), -(m_length / 2)},
                           {0.0f, 1.0f},
                           {-1.0f, 0.0f, 0.0f}},
                          {{-(m_width / 2), -(m_height / 2), -(m_length / 2)},
                           {0.0f, 0.0f},
                           {-1.0f, 0.0f, 0.0f}}));

  // fifth face (PY)
  addPolygon(Npi::Polygon({{-(m_width / 2), (m_height / 2), -(m_length / 2)},
                           {0.0f, 0.0f},
                           {0.0f, +1.0f, 0.0f}},
                          {{-(m_width / 2), (m_height / 2), (m_length / 2)},
                           {1.0f, 0.0f},
                           {0.0f, +1.0f, 0.0f}},
                          {{(m_width / 2), (m_height / 2), (m_length / 2)},
                           {1.0f, 1.0f},
                           {0.0f, +1.0f, 0.0f}}));

  addPolygon(Npi::Polygon({{(m_width / 2), (m_height / 2), (m_length / 2)},
                           {1.0f, 1.0f},
                           {0.0f, +1.0f, 0.0f}},
                          {{(m_width / 2), (m_height / 2), -(m_length / 2)},
                           {0.0f, 1.0f},
                           {0.0f, +1.0f, 0.0f}},
                          {{-(m_width / 2), (m_height / 2), -(m_length / 2)},
                           {0.0f, 0.0f},
                           {0.0f, +1.0f, 0.0f}}));

  // sixth face (MY)
  addPolygon(Npi::Polygon({{-(m_width / 2), -(m_height / 2), -(m_length / 2)},
                           {0.0f, 0.0f},
                           {0.0f, -1.0f, 0.0f}},
                          {{(m_width / 2), -(m_height / 2), -(m_length / 2)},
                           {1.0f, 0.0f},
                           {0.0f, -1.0f, 0.0f}},
                          {{(m_width / 2), -(m_height / 2), (m_length / 2)},
                           {1.0f, 1.0f},
                           {0.0f, -1.0f, 0.0f}}));

  addPolygon(Npi::Polygon({{(m_width / 2), -(m_height / 2), (m_length / 2)},
                           {1.0f, 1.0f},
                           {0.0f, -1.0f, 0.0f}},
                          {{-(m_width / 2), -(m_height / 2), (m_length / 2)},
                           {0.0f, 1.0f},
                           {0.0f, -1.0f, 0.0f}},
                          {{-(m_width / 2), -(m_height / 2), -(m_length / 2)},
                           {0.0f, 0.0f},
                           {0.0f, -1.0f, 0.0f}}));

  updateVBO();
}
} // namespace Npi