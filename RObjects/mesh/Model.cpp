#include "Model.hpp"
#include "OBJloader.hpp"


namespace Npi
{
    bool Model::LoadFromFile(std::string path)
    {
        objl::Loader loader;
        if (!loader.LoadFile(path))
        {
            printf("MDL: Failed");
            return false;
        }

        for (unsigned m = 0; m < loader.LoadedMaterials.size(); m++)
        {
            Material tm(loader.LoadedMaterials[m].Ka.X, loader.LoadedMaterials[m].Ka.Y, loader.LoadedMaterials[m].Ka.Z, loader.LoadedMaterials[m].Kd.X, loader.LoadedMaterials[m].Kd.Y, loader.LoadedMaterials[m].Ks.Z, loader.LoadedMaterials[m].Ks.X, loader.LoadedMaterials[m].Ks.Y, loader.LoadedMaterials[m].Ks.Z);
            this->setMaterial(tm);
            //
        }
        for (unsigned n = 0; n < loader.LoadedMeshes.size(); n++)
        {
            printf("\x1b[3;1HLoading Model: %i/%i\x1b[K", n, (int)loader.LoadedMeshes.size());
            objl::Mesh curMesh = loader.LoadedMeshes[n];
            for (unsigned int j = 0; j < curMesh.Vertices.size() - 2; j += 3) {
                printf("\x1b[4;1HLoading Model Vertices: %i/%i\x1b[K", j, (int)curMesh.Vertices.size() - 2);
                addPolygon(Npi::Polygon(
                    { {
                        curMesh.Vertices[j].Position.X,
                        curMesh.Vertices[j].Position.Y,
                        curMesh.Vertices[j].Position.Z
                    }, {
                        curMesh.Vertices[j].TextureCoordinate.X,
                        curMesh.Vertices[j].TextureCoordinate.Y
                    }, {
                        curMesh.Vertices[j].Normal.X,
                        curMesh.Vertices[j].Normal.Y,
                        curMesh.Vertices[j].Normal.Z
                    } },

                    { {
                        curMesh.Vertices[j + 1].Position.X,
                        curMesh.Vertices[j + 1].Position.Y,
                        curMesh.Vertices[j + 1].Position.Z
                    }, {
                        curMesh.Vertices[j + 1].TextureCoordinate.X,
                        curMesh.Vertices[j + 1].TextureCoordinate.Y
                    }, {
                        curMesh.Vertices[j + 1].Normal.X,
                        curMesh.Vertices[j + 1].Normal.Y,
                        curMesh.Vertices[j + 1].Normal.Z
                    } },

                    { {
                        curMesh.Vertices[j + 2].Position.X,
                        curMesh.Vertices[j + 2].Position.Y,
                        curMesh.Vertices[j + 2].Position.Z
                    }, {
                        curMesh.Vertices[j + 2].TextureCoordinate.X,
                        curMesh.Vertices[j + 2].TextureCoordinate.Y
                    }, {
                        curMesh.Vertices[j + 2].Normal.X,
                        curMesh.Vertices[j + 2].Normal.Y,
                        curMesh.Vertices[j + 2].Normal.Z
                    } }
                    ));
            }
        }
        updateVBO();
        return true;
    }

    void Model::draw(Npi::RenderContext t_context) {
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
            C3D_CullFace(GPU_CULL_NONE);
            C3D_DrawArrays(GPU_TRIANGLES, 0, this->GetVertecies().size());
        }
    }

} // namespace Npi
