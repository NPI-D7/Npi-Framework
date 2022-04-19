#include "SkyBox.hpp"

Npi::Texture sktex;

C3D_Tex skybox_tex;
C3D_TexCube skybox_cube;

static bool loadTextureFromFile(C3D_Tex* tex, C3D_TexCube* cube, const char* path)
{
	FILE* f = fopen(path, "rb");
	if (!f)
		return false;

	Tex3DS_Texture t3x = Tex3DS_TextureImportStdio(f, tex, cube, false);
	fclose(f);
	if (!t3x)
		return false;

	// Delete the t3x object since we don't need it
	Tex3DS_TextureFree(t3x);
	return true;
}

namespace Npi {
    SkyBox::SkyBox(float t_length, float t_width, float t_height) {
        m_length = t_length;
        m_width = t_width;
        m_height = t_height;
        update();
    }

    void SkyBox::SetTex(std::string path)
    {
        if (!loadTextureFromFile(&skybox_tex, &skybox_cube, path.c_str()))
        {
            printf("Tex not found");
            svcBreak(USERBREAK_PANIC);
        }
		    

    C3D_TexSetFilter(&skybox_tex, GPU_LINEAR, GPU_LINEAR);
	C3D_TexSetWrap(&skybox_tex, GPU_CLAMP_TO_EDGE, GPU_CLAMP_TO_EDGE);
	C3D_TexBind(0, &skybox_tex);
    sktex.setTextuere(&skybox_tex);

	// Configure the first fragment shading substage to use the texture color.
	// See https://www.opengl.org/sdk/docs/man2/xhtml/glTexEnv.xml for more insight
	C3D_TexEnv* env = C3D_GetTexEnv(0);
	C3D_TexEnvInit(env);
	C3D_TexEnvSrc(env, C3D_Both, GPU_TEXTURE0);
	C3D_TexEnvFunc(env, C3D_Both, GPU_REPLACE);
    }

    void SkyBox::setLength(float t_length) {
        m_length = t_length;
    }

    float SkyBox::getLength() {
        return m_length;
    }

    void SkyBox::setWidth(float t_width) {
        m_width = t_width;
    }

    float SkyBox::getWidth() {
        return m_width;
    }

    void SkyBox::setHeight(float t_height) {
        m_height = t_height;
    }

    float SkyBox::getHeight() {
        return m_height;
    }

    // private methods
    void SkyBox::update() {
        clearVertices();

        // first face (PZ)
        addPolygon(Npi::Polygon(
            { {-(m_width / 2), -(m_height / 2), (m_length / 2)}, {0.0f, 0.0f}, {0.0f, 0.0f, +1.0f} },
            { {(m_width / 2), -(m_height / 2), (m_length / 2)}, {-1.0f, 0.0f}, {0.0f, 0.0f, +1.0f} },
            { {(m_width / 2), (m_height / 2), (m_length / 2)}, {-1.0f, -1.0f}, {0.0f, 0.0f, +1.0f} }
        ));

        addPolygon(Npi::Polygon(
            { {(m_width / 2), (m_height / 2), (m_length / 2)}, {-1.0f, -1.0f}, {0.0f, 0.0f, +1.0f} },
            { {-(m_width / 2), (m_height / 2), (m_length / 2)}, {0.0f, -1.0f}, {0.0f, 0.0f, +1.0f} },
            { {-(m_width / 2), -(m_height / 2), (m_length / 2)}, {0.0f, 0.0f}, {0.0f, 0.0f, +1.0f} }
        ));

        // second face (MZ)
        addPolygon(Npi::Polygon(
            { {-(m_width / 2), -(m_height / 2), -(m_length / 2)}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f} },
            { {-(m_width / 2), (m_height / 2), -(m_length / 2)}, {-1.0f, 0.0f}, {0.0f, 0.0f, -1.0f} },
            { {(m_width / 2), (m_height / 2), -(m_length / 2)}, {-1.0f, -1.0f}, {0.0f, 0.0f, -1.0f} }
        ));

        addPolygon(Npi::Polygon(
            { {(m_width / 2), (m_height / 2), -(m_length / 2)}, {-1.0f, -1.0f}, {0.0f, 0.0f, -1.0f} },
            { {(m_width / 2), -(m_height / 2), -(m_length / 2)}, {0.0f, -1.0f}, {0.0f, 0.0f, -1.0f} },
            { {-(m_width / 2), -(m_height / 2), -(m_length / 2)}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f} }
        ));

        // third face (PX)
        addPolygon(Npi::Polygon(
            { {(m_width / 2), -(m_height / 2), -(m_length / 2)}, {0.0f, 0.0f}, {+1.0f, 0.0f, 0.0f} },
            { {(m_width / 2), (m_height / 2), -(m_length / 2)}, {-1.0f, 0.0f}, {+1.0f, 0.0f, 0.0f} },
            { {(m_width / 2), (m_height / 2), (m_length / 2)}, {-1.0f, -1.0f}, {+1.0f, 0.0f, 0.0f} }
        ));

        addPolygon(Npi::Polygon(
            { {(m_width / 2), (m_height / 2), (m_length / 2)}, {-1.0f, -1.0f}, {+1.0f, 0.0f, 0.0f} },
            { {(m_width / 2), -(m_height / 2), (m_length / 2)}, {0.0f, -1.0f}, {+1.0f, 0.0f, 0.0f} },
            { {(m_width / 2), -(m_height / 2), -(m_length / 2)}, {0.0f, 0.0f}, {+1.0f, 0.0f, 0.0f} }
        ));

        // fourth face (MX)
        addPolygon(Npi::Polygon(
            { {-(m_width / 2), -(m_height / 2), -(m_length / 2)}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f} },
            { {-(m_width / 2), -(m_height / 2), (m_length / 2)}, {-1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f} },
            { {-(m_width / 2), (m_height / 2), (m_length / 2)}, {-1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f} }
        ));

        addPolygon(Npi::Polygon(
            { {-(m_width / 2), (m_height / 2), (m_length / 2)}, {-1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f} },
            { {-(m_width / 2), (m_height / 2), -(m_length / 2)}, {0.0f, -1.0f}, {-1.0f, 0.0f, 0.0f} },
            { {-(m_width / 2), -(m_height / 2), -(m_length / 2)}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f} }
        ));

        // fifth face (PY)
        addPolygon(Npi::Polygon(
            { {-(m_width / 2), (m_height / 2), -(m_length / 2)}, {0.0f, 0.0f}, {0.0f, +1.0f, 0.0f} },
            { {-(m_width / 2), (m_height / 2), (m_length / 2)}, {-1.0f, 0.0f}, {0.0f, +1.0f, 0.0f} },
            { {(m_width / 2), (m_height / 2), (m_length / 2)}, {-1.0f, -1.0f}, {0.0f, +1.0f, 0.0f} }
        ));

        addPolygon(Npi::Polygon(
            { {(m_width / 2), (m_height / 2), (m_length / 2)}, {-1.0f, -1.0f}, {0.0f, +1.0f, 0.0f} },
            { {(m_width / 2), (m_height / 2), -(m_length / 2)}, {0.0f, -1.0f}, {0.0f, +1.0f, 0.0f} },
            { {-(m_width / 2), (m_height / 2), -(m_length / 2)}, {0.0f, 0.0f}, {0.0f, +1.0f, 0.0f} }
        ));

        // sixth face (MY)
        addPolygon(Npi::Polygon(
            { {-(m_width / 2), -(m_height / 2), -(m_length / 2)}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f} },
            { {(m_width / 2), -(m_height / 2), -(m_length / 2)}, {-1.0f, 0.0f}, {0.0f, -1.0f, 0.0f} },
            { {(m_width / 2), -(m_height / 2), (m_length / 2)}, {-1.0f, -1.0f}, {0.0f, -1.0f, 0.0f} }
        ));

        addPolygon(Npi::Polygon(
            { {(m_width / 2), -(m_height / 2), (m_length / 2)}, {-1.0f, -1.0f}, {0.0f, -1.0f, 0.0f} },
            { {-(m_width / 2), -(m_height / 2), (m_length / 2)}, {0.0f, -1.0f}, {0.0f, -1.0f, 0.0f} },
            { {-(m_width / 2), -(m_height / 2), -(m_length / 2)}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f} }
        ));

        updateVBO();
    }

    void SkyBox::draw(Npi::RenderContext t_context) {
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
                C3D_TexSetFilter(sktex.getTexture(), GPU_LINEAR, GPU_LINEAR);
                C3D_TexBind(0, sktex.getTexture());
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
            C3D_CullFace(GPU_CULL_FRONT_CCW);
            C3D_DrawArrays(GPU_TRIANGLES, 0, this->GetVertecies().size());
        }
    }
} /* Npi */