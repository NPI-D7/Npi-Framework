#include "cube.hpp"

namespace Npi {
    Cube::Cube(float t_length, float t_width, float t_height) {
        m_length = t_length;
        m_width = t_width;
        m_height = t_height;
        update();
    }

    void Cube::setLength(float t_length) {
        m_length = t_length;
    }

    float Cube::getLength() {
        return m_length;
    }

    void Cube::setWidth(float t_width) {
        m_width = t_width;
    }

    float Cube::getWidth() {
        return m_width;
    }

    void Cube::setHeight(float t_height) {
        m_height = t_height;
    }

    float Cube::getHeight() {
        return m_height;
    }

    // private methods
    void Cube::update() {
        clearVertices();

        // first face (PZ)
        addPolygon(Npi::Polygon(
            { {-(m_width / 2), -(m_height / 2), (m_length / 2)}, {0.0f, 0.0f}, {0.0f, 0.0f, +1.0f} },
            { {(m_width / 2), -(m_height / 2), (m_length / 2)}, {1.0f, 0.0f}, {0.0f, 0.0f, +1.0f} },
            { {(m_width / 2), (m_height / 2), (m_length / 2)}, {1.0f, 1.0f}, {0.0f, 0.0f, +1.0f} }
        ));

        addPolygon(Npi::Polygon(
            { {(m_width / 2), (m_height / 2), (m_length / 2)}, {1.0f, 1.0f}, {0.0f, 0.0f, +1.0f} },
            { {-(m_width / 2), (m_height / 2), (m_length / 2)}, {0.0f, 1.0f}, {0.0f, 0.0f, +1.0f} },
            { {-(m_width / 2), -(m_height / 2), (m_length / 2)}, {0.0f, 0.0f}, {0.0f, 0.0f, +1.0f} }
        ));

        // second face (MZ)
        addPolygon(Npi::Polygon(
            { {-(m_width / 2), -(m_height / 2), -(m_length / 2)}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f} },
            { {-(m_width / 2), (m_height / 2), -(m_length / 2)}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f} },
            { {(m_width / 2), (m_height / 2), -(m_length / 2)}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f} }
        ));

        addPolygon(Npi::Polygon(
            { {(m_width / 2), (m_height / 2), -(m_length / 2)}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f} },
            { {(m_width / 2), -(m_height / 2), -(m_length / 2)}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f} },
            { {-(m_width / 2), -(m_height / 2), -(m_length / 2)}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f} }
        ));

        // third face (PX)
        addPolygon(Npi::Polygon(
            { {(m_width / 2), -(m_height / 2), -(m_length / 2)}, {0.0f, 0.0f}, {+1.0f, 0.0f, 0.0f} },
            { {(m_width / 2), (m_height / 2), -(m_length / 2)}, {1.0f, 0.0f}, {+1.0f, 0.0f, 0.0f} },
            { {(m_width / 2), (m_height / 2), (m_length / 2)}, {1.0f, 1.0f}, {+1.0f, 0.0f, 0.0f} }
        ));

        addPolygon(Npi::Polygon(
            { {(m_width / 2), (m_height / 2), (m_length / 2)}, {1.0f, 1.0f}, {+1.0f, 0.0f, 0.0f} },
            { {(m_width / 2), -(m_height / 2), (m_length / 2)}, {0.0f, 1.0f}, {+1.0f, 0.0f, 0.0f} },
            { {(m_width / 2), -(m_height / 2), -(m_length / 2)}, {0.0f, 0.0f}, {+1.0f, 0.0f, 0.0f} }
        ));

        // fourth face (MX)
        addPolygon(Npi::Polygon(
            { {-(m_width / 2), -(m_height / 2), -(m_length / 2)}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f} },
            { {-(m_width / 2), -(m_height / 2), (m_length / 2)}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f} },
            { {-(m_width / 2), (m_height / 2), (m_length / 2)}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f} }
        ));

        addPolygon(Npi::Polygon(
            { {-(m_width / 2), (m_height / 2), (m_length / 2)}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f} },
            { {-(m_width / 2), (m_height / 2), -(m_length / 2)}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f} },
            { {-(m_width / 2), -(m_height / 2), -(m_length / 2)}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f} }
        ));

        // fifth face (PY)
        addPolygon(Npi::Polygon(
            { {-(m_width / 2), (m_height / 2), -(m_length / 2)}, {0.0f, 0.0f}, {0.0f, +1.0f, 0.0f} },
            { {-(m_width / 2), (m_height / 2), (m_length / 2)}, {1.0f, 0.0f}, {0.0f, +1.0f, 0.0f} },
            { {(m_width / 2), (m_height / 2), (m_length / 2)}, {1.0f, 1.0f}, {0.0f, +1.0f, 0.0f} }
        ));

        addPolygon(Npi::Polygon(
            { {(m_width / 2), (m_height / 2), (m_length / 2)}, {1.0f, 1.0f}, {0.0f, +1.0f, 0.0f} },
            { {(m_width / 2), (m_height / 2), -(m_length / 2)}, {0.0f, 1.0f}, {0.0f, +1.0f, 0.0f} },
            { {-(m_width / 2), (m_height / 2), -(m_length / 2)}, {0.0f, 0.0f}, {0.0f, +1.0f, 0.0f} }
        ));

        // sixth face (MY)
        addPolygon(Npi::Polygon(
            { {-(m_width / 2), -(m_height / 2), -(m_length / 2)}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f} },
            { {(m_width / 2), -(m_height / 2), -(m_length / 2)}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f} },
            { {(m_width / 2), -(m_height / 2), (m_length / 2)}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f} }
        ));

        addPolygon(Npi::Polygon(
            { {(m_width / 2), -(m_height / 2), (m_length / 2)}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f} },
            { {-(m_width / 2), -(m_height / 2), (m_length / 2)}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f} },
            { {-(m_width / 2), -(m_height / 2), -(m_length / 2)}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f} }
        ));

        updateVBO();
    }

    void Cube::draw(Npi::RenderContext t_context) {
        if (t_context.getMode() == Npi::RenderContext::Mode::_3D) {
            // manipulate modelview matrix
            Mtx_Identity(&t_context.getModelMatrix());
            Mtx_Translate(&t_context.getModelMatrix(), this->getPositionX(), this->getPositionY(),  -1.87 - this->getPositionZ(), true);
            Mtx_RotateX(&t_context.getModelMatrix(), this->getPitch(), true);
            Mtx_RotateY(&t_context.getModelMatrix(), this->getYaw(), true);
            Mtx_RotateZ(&t_context.getModelMatrix(), this->getRoll(), true);
            Mtx_Scale(&t_context.getModelMatrix(), this->getScaleX(), this->getScaleY(), this->getScaleZ());

            // set material
            C3D_LightEnvMaterial(&t_context.getLightEnvironment(), this->getMaterial().getMaterial());

            if (this->isTextureBound()) {
                // enable textures
                t_context.enableTextures(true);

                // bind the texture
                C3D_TexSetFilter(this->getTexture().getTexture(), GPU_LINEAR, GPU_LINEAR);
                C3D_TexBind(0, this->getTexture().getTexture());
            } else {
                // disable textures
                t_context.enableTextures(false);
            }

            // create buffer
            C3D_BufInfo* bufInfo = C3D_GetBufInfo();
            BufInfo_Init(bufInfo);
            BufInfo_Add(bufInfo, this->GetVbo(), sizeof(Npi::Polygon::Vertex), 3, 0x210);

            // update the uniforms
            C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, t_context.getModelUniform(),  &t_context.getModelMatrix());

            // draw the VBO
            C3D_DrawArrays(GPU_TRIANGLES, 0, this->GetVertecies().size());
        }
    }
} /* Npi */