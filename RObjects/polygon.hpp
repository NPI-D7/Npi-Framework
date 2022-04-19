#pragma once
#include <tuple>
#include <vector>
#include "material.hpp"
#include "RObject.hpp"
#include "color.hpp"
#include "texture.hpp"
#include "vertex.hpp"

namespace Npi
{
    class Polygon {
    public:
        /**
         * @brief Represents a vertex of a polygon
         */
        struct Vertex {
            float position[3]; ///< the position
            float texcoord[2]; ///< the texture coordinates
            float normal[3];   ///< the normals
            float color[4];    ///< the colors
        };

        /**
         * @brief Initializes the polygon
         * @param t_vertex0 The first vertex
         * @param t_vertex1 The second vertex
         * @param t_vertex2 The third vertex
         */
        Polygon(Npi::Polygon::Vertex t_vertex0, Npi::Polygon::Vertex t_vertex1, Npi::Polygon::Vertex t_vertex2);

        /**
         * @brief Sets the vertices of the polygon
         * @param t_vertex0 The first vertex
         * @param t_vertex1 The second vertex
         * @param t_vertex2 The third vertex
         */
        void setVertices(Npi::Polygon::Vertex t_vertex0, Npi::Polygon::Vertex t_vertex1, Npi::Polygon::Vertex t_vertex2);

        /**
         * @brief Returns a vertex given it's id
         * @param  t_id The id of the vertex
         * @return      The vertex at the given id
         */
        Npi::Polygon::Vertex getVertex(int t_id);

    private:
        /* data */
        Npi::Polygon::Vertex m_vertices[3];
    };

}