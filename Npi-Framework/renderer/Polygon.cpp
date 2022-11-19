#include <Polygon.hpp>

int polygoncount = 0;

namespace Npi {
Polygon::Polygon(Npi::Polygon::Vertex t_vertex0, Npi::Polygon::Vertex t_vertex1,
                 Npi::Polygon::Vertex t_vertex2) {
  polygoncount++;
  setVertices(t_vertex0, t_vertex1, t_vertex2);
}

/**
 * @brief Sets the vertices of the polygon
 * @param t_vertex0 The first vertex
 * @param t_vertex1 The second vertex
 * @param t_vertex2 The third vertex
 */
void Polygon::setVertices(Npi::Polygon::Vertex t_vertex0,
                          Npi::Polygon::Vertex t_vertex1,
                          Npi::Polygon::Vertex t_vertex2) {
  m_vertices[0] = t_vertex0;
  m_vertices[1] = t_vertex1;
  m_vertices[2] = t_vertex2;
}

/**
 * @brief Returns a vertex given it's id
 * @param  t_id The id of the vertex
 * @return      The vertex at the given id
 */
Npi::Polygon::Vertex Polygon::getVertex(int t_id) { return m_vertices[t_id]; }
} // namespace Npi