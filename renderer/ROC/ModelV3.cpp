#include "ModelV3.hpp"

#include <filesystem>

#include "nmdl.hpp"

namespace Npi {
bool ModelV3::LoadFromFile(std::string path) {
  NMDL::Model m;
  std::cout << "Loading Model" << std::endl;
  NMDL::Load(m, path);
  std::cout << "Setting Up VBO" << std::endl;

  for (size_t i = 0; i < m.Meshes.size(); i++) {
    Npi::Mesh* temp = new Npi::Mesh;
    auto mesh = &m.Meshes[i];
    NMDL::Material mat;
    for (size_t j = 0; j < m.Materials.size(); j++) {
      if (m.Materials[j].MaterialID == mesh->MaterialID) {
        mat = m.Materials[j];
        break;
      }
    }
    if (mat.DiffuseTex.ID != 0) {
      Npi::Texture tex;
      for (auto& it : m.Textures) {
        if (it.ID == mat.DiffuseTex) {
          tex.loadFromBufferEx(it.Buffer.data(), it.Width, it.Height);
          temp->bindTexture(tex);
        }
      }
    }
    temp->getMaterial().setDiffuse(Npi::Color(mat.DiffuseColor.x * 255.f,
                                              mat.DiffuseColor.y * 255.f,
                                              mat.DiffuseColor.z * 255.f));
    temp->getMaterial().setSpecular0(Npi::Color(mat.SpecularColor.x * 255,
                                                mat.SpecularColor.y * 255,
                                                mat.SpecularColor.z * 255));
    if ((mesh->NumVertices % 3) != 0 || mesh->NumVertices == 0) {
      return false;
    }
    for (unsigned int j = 0; j < mesh->NumVertices - 2; j += 3) {
      temp->addPolygon(Npi::Polygon(
          {{mesh->Vertices[j].Pos.x, mesh->Vertices[j].Pos.y,
            mesh->Vertices[j].Pos.z},
           {mesh->Vertices[j].UV.x, mesh->Vertices[j].UV.y},
           {mesh->Vertices[j].Normal.x, mesh->Vertices[j].Normal.y,
            mesh->Vertices[j].Normal.z}},

          {{mesh->Vertices[j + 1].Pos.x, mesh->Vertices[j + 1].Pos.y,
            mesh->Vertices[j + 1].Pos.z},
           {mesh->Vertices[j + 1].UV.x, mesh->Vertices[j + 1].UV.y},
           {mesh->Vertices[j + 1].Normal.x, mesh->Vertices[j + 1].Normal.y,
            mesh->Vertices[j + 1].Normal.z}},

          {{mesh->Vertices[j + 2].Pos.x, mesh->Vertices[j + 2].Pos.y,
            mesh->Vertices[j + 2].Pos.z},
           {mesh->Vertices[j + 2].UV.x, mesh->Vertices[j + 2].UV.y},
           {mesh->Vertices[j + 2].Normal.x, mesh->Vertices[j + 2].Normal.y,

            mesh->Vertices[j + 2].Normal.z}}));
    }
    temp->updateVBO();
    this->mesches.push_back(temp);
  }
  return true;
}

void ModelV3::draw(Npi::RenderContext t_context) {
  for (size_t i = 0; i < this->mesches.size(); i++) {
    mesches[i]->draw(t_context);
  }
}

void ModelV3::rotateYaw(float t_delta, bool t_radians) {
  for (size_t i = 0; i < this->mesches.size(); i++) {
    mesches[i]->rotateYaw(t_delta, t_radians);
  }
}

void ModelV3::setYaw(float t_rotation, bool t_radians) {
  for (size_t i = 0; i < this->mesches.size(); i++) {
    mesches[i]->setYaw(t_rotation, t_radians);
  }
}

void ModelV3::setScale(float t_scaleX, float t_scaleY, float t_scaleZ) {
  for (size_t i = 0; i < this->mesches.size(); i++) {
    mesches[i]->setScale(t_scaleX, t_scaleY, t_scaleZ);
  }
}

}  // namespace Npi