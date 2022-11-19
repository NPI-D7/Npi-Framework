#include "ModelV2.hpp"
#include "OBJloader.hpp"
#include <filesystem>

namespace Npi {
bool ModelV2::LoadFromFile(std::string path) {
  objl::Loader loader;
  if (!loader.LoadFile(path)) {
    printf("MDL: Failed");
    return false;
  }

  for (unsigned n = 0; n < loader.LoadedMeshes.size(); n++) {
    Npi::Mesh *temp = new Npi::Mesh;
    printf("\x1b[3;1HLoading Model: %i/%i\x1b[K", n,
           (int)loader.LoadedMeshes.size());
    objl::Mesh curMesh = loader.LoadedMeshes[n];
    // temp->getMaterial().setAmbient(Npi::Color(curMesh.MeshMaterial.Ka.X *
    // 255,
    //                                           curMesh.MeshMaterial.Ka.Y *
    //                                           255, curMesh.MeshMaterial.Ka.Z
    //                                           * 255));
    temp->getMaterial().setDiffuse(Npi::Color(curMesh.MeshMaterial.Kd.X * 255,
                                              curMesh.MeshMaterial.Kd.Y * 255,
                                              curMesh.MeshMaterial.Kd.Z * 255));
    temp->getMaterial().setSpecular0(Npi::Color(
        curMesh.MeshMaterial.Ks.X * 255, curMesh.MeshMaterial.Ks.Y * 255,
        curMesh.MeshMaterial.Ks.Z * 255));

    auto tp = std::filesystem::path(path);
    bool ldd = false;
    if (curMesh.MeshMaterial.map_Kd != "") {
      Npi::Texture tex;

      ldd = tex.loadFromFile(tp.remove_filename().string() +
                             curMesh.MeshMaterial.map_Kd);
      temp->bindTexture(tex);
    }
    printf("\x1b[5;1HTexture: %s%s (%s)\x1b[K", tp.c_str(),
           curMesh.MeshMaterial.map_Kd.c_str(), ldd ? "true" : "false");
    for (unsigned int j = 0; j < curMesh.Vertices.size() - 2; j += 3) {
      printf("\x1b[4;1HLoading Model Vertices: %i/%i\x1b[K", j,
             (int)curMesh.Vertices.size() - 2);
      temp->addPolygon(Npi::Polygon(
          {{curMesh.Vertices[j].Position.X, curMesh.Vertices[j].Position.Y,
            curMesh.Vertices[j].Position.Z},
           {curMesh.Vertices[j].TextureCoordinate.X,
            curMesh.Vertices[j].TextureCoordinate.Y},
           {curMesh.Vertices[j].Normal.X, curMesh.Vertices[j].Normal.Y,
            curMesh.Vertices[j].Normal.Z}},

          {{curMesh.Vertices[j + 1].Position.X,
            curMesh.Vertices[j + 1].Position.Y,
            curMesh.Vertices[j + 1].Position.Z},
           {curMesh.Vertices[j + 1].TextureCoordinate.X,
            curMesh.Vertices[j + 1].TextureCoordinate.Y},
           {curMesh.Vertices[j + 1].Normal.X, curMesh.Vertices[j + 1].Normal.Y,
            curMesh.Vertices[j + 1].Normal.Z}},

          {{curMesh.Vertices[j + 2].Position.X,
            curMesh.Vertices[j + 2].Position.Y,
            curMesh.Vertices[j + 2].Position.Z},
           {curMesh.Vertices[j + 2].TextureCoordinate.X,
            curMesh.Vertices[j + 2].TextureCoordinate.Y},
           {curMesh.Vertices[j + 2].Normal.X, curMesh.Vertices[j + 2].Normal.Y,

            curMesh.Vertices[j + 2].Normal.Z}}));
    }
    temp->updateVBO();
    this->mesches.push_back(temp);
  }

  return true;
}

void ModelV2::draw(Npi::RenderContext t_context) {
  for (size_t i = 0; i < this->mesches.size(); i++) {
    mesches[i]->draw(t_context);
  }
}

void ModelV2::rotateYaw(float t_delta, bool t_radians)
{
  for (size_t i = 0; i < this->mesches.size(); i++) {
    mesches[i]->rotateYaw(t_delta, t_radians);
  }
}

void ModelV2::setYaw(float t_rotation, bool t_radians)
{
  for (size_t i = 0; i < this->mesches.size(); i++) {
    mesches[i]->setYaw(t_rotation, t_radians);
  }
}

void ModelV2::setScale(float t_scaleX, float t_scaleY, float t_scaleZ)
{
  for (size_t i = 0; i < this->mesches.size(); i++) {
    mesches[i]->setScale(t_scaleX, t_scaleY, t_scaleZ);
  }
}

} // namespace Npi