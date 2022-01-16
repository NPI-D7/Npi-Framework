#include "mesh.hpp"

namespace Npi {
    Mesh::Polygon::Polygon(Npi::Mesh::Polygon::Vertex t_vertex0, Npi::Mesh::Polygon::Vertex t_vertex1, Npi::Mesh::Polygon::Vertex t_vertex2) {
        setVertices(t_vertex0, t_vertex1, t_vertex2);
    }

    /**
     * @brief Sets the vertices of the polygon
     * @param t_vertex0 The first vertex
     * @param t_vertex1 The second vertex
     * @param t_vertex2 The third vertex
     */
    void Mesh::Polygon::setVertices(Npi::Mesh::Polygon::Vertex t_vertex0, Npi::Mesh::Polygon::Vertex t_vertex1, Npi::Mesh::Polygon::Vertex t_vertex2) {
        m_vertices[0] = t_vertex0;
        m_vertices[1] = t_vertex1;
        m_vertices[2] = t_vertex2;
    }

    /**
     * @brief Returns a vertex given it's id
     * @param  t_id The id of the vertex
     * @return      The vertex at the given id
     */
    Npi::Mesh::Polygon::Vertex Mesh::Polygon::getVertex(int t_id) {
        return m_vertices[t_id];
    }
} /* Npi */