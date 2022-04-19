#pragma once
#include "mesh.hpp"

namespace Npi {
    
    class Model: public Npi::Mesh {
    public:
        bool LoadFromFile(std::string path);
        void draw(Npi::RenderContext t_context);

    private:
        void update();
        float m_length, m_width, m_height;
    };
} /* Npi */
