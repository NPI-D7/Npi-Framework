#pragma once
#include "mesh.hpp"

namespace Npi {
    
    class Cube: public Npi::Mesh {
    public:
        Cube(float t_length, float t_width, float t_height);
        void setLength(float t_length);
        float getLength();
        void setWidth(float t_width);
        float getWidth();

        void setHeight(float t_height);
        float getHeight();
        /**
         * @brief Draws the mesh
         * @param t_context The RenderContext
         */
        void draw(Npi::RenderContext t_context);

    private:
        void update();
        float m_length, m_width, m_height;
    };
} /* Npi */
