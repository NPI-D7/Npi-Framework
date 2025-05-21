#include <npi/nmdl.hpp>

namespace NMDL {
void Load(Model& m, const std::string& filename) {
  std::ifstream iff(filename);
  if (!iff) {
    throw std::runtime_error("unable to load " + filename);
  }
  iff.read((char*)&m.Magic, sizeof(m.Magic));
  iff.read((char*)&m.Magic, sizeof(m.Version));
  iff.read((char*)&m.Magic, sizeof(m.DataBeg));
  iff.read((char*)&m.ReferenceTabLen, sizeof(m.ReferenceTabLen));
  for (PD::u32 i = 0; i < m.ReferenceTabLen; i++) {
    Reference r;
    iff.read((char*)&r.ID, sizeof(r.ID));
    iff.read((char*)&r.StartPos, sizeof(r.StartPos));
    iff.read((char*)&r.NameLen, sizeof(r.NameLen));
    iff.read(r.Name.data(), r.NameLen);
  }
  iff.read((char*)&m.NumMeshes, sizeof(m.NumMeshes));
  for (PD::u32 i = 0; i < m.NumMeshes; i++) {
    Mesh r;
    iff.read((char*)&r.MeshID, sizeof(r.MeshID));
    iff.read((char*)&r.NumVertices, sizeof(r.NumVertices));
    r.Vertices.resize(r.NumVertices);
    iff.read((char*)r.Vertices.data(), r.NumVertices * sizeof(Vertex));
    iff.read((char*)&r.NumIndices, sizeof(r.NumIndices));
    r.Indices.resize(r.NumIndices);
    iff.read((char*)r.Indices.data(), r.NumIndices * sizeof(PD::u32));
    iff.read((char*)&r.MaterialID, sizeof(r.MaterialID));
    m.Meshes.push_back(r);
  }
  iff.read((char*)&m.NumMaterials, sizeof(m.NumMaterials));
  m.Materials.resize(m.NumMaterials);
  iff.read((char*)m.Materials.data(), m.NumMaterials * sizeof(Material));
  iff.read((char*)&m.NumTextures, sizeof(m.NumTextures));
  for (size_t i = 0; i < m.NumTextures; i++) {
    Texture t;
    iff.read((char*)&t.ID, sizeof(t.ID));
    iff.read((char*)&t.Width, sizeof(t.Width));
    iff.read((char*)&t.Height, sizeof(t.Height));
    t.Buffer.resize(t.Width * t.Height * 4);
    iff.read((char*)t.Buffer.data(), t.Width * t.Height * 4);
  }
}

void Save(Model& mdl, const std::string& filename) {
  std::ofstream o(filename);
  o.write((const char*)&mdl.Magic, sizeof(mdl.Magic));
  o.write((const char*)&mdl.Version, sizeof(mdl.Version));
  o.write((const char*)&mdl.DataBeg, sizeof(mdl.DataBeg));
  mdl.ReferenceTabLen = mdl.ReferenceTab.size();
  o.write((const char*)&mdl.ReferenceTabLen, sizeof(mdl.ReferenceTabLen));
  for (auto& it : mdl.ReferenceTab) {
    o.write((const char*)&it.ID.ID, sizeof(it.ID));
    o.write((const char*)&it.StartPos, sizeof(it.StartPos));
    o.write((const char*)&it.NameLen, sizeof(it.NameLen));
    o.write((const char*)it.Name.data(), it.Name.size());
  }
  mdl.NumMeshes = mdl.Meshes.size();
  o.write((const char*)&mdl.NumMeshes, sizeof(mdl.NumMeshes));
  for (auto& it : mdl.Meshes) {
    o.write((const char*)&it.MeshID.ID, sizeof(it.MeshID));
    it.NumVertices = it.Vertices.size();
    o.write((const char*)&it.NumVertices, sizeof(it.NumVertices));
    o.write((const char*)it.Vertices.data(), it.NumVertices * sizeof(Vertex));
    it.NumIndices = it.Indices.size();
    o.write((const char*)&it.NumIndices, sizeof(it.NumIndices));
    o.write((const char*)it.Indices.data(), it.NumIndices * sizeof(PD::u32));
    o.write((const char*)&it.MaterialID.ID, sizeof(it.MaterialID));
  }
  mdl.NumMaterials = mdl.Materials.size();
  o.write((const char*)&mdl.NumMaterials, sizeof(mdl.NumMaterials));
  for (auto& it : mdl.Materials) {
    o.write((const char*)&it, sizeof(Material));
  }
  mdl.NumTextures = mdl.Textures.size();
  o.write((const char*)&mdl.NumTextures, sizeof(mdl.NumTextures));
  for (auto& it : mdl.Textures) {
    o.write((const char*)&it.ID, sizeof(it.ID));
    o.write((const char*)&it.Width, sizeof(it.Width));
    o.write((const char*)&it.Height, sizeof(it.Height));
    o.write((const char*)it.Buffer.data(), it.Width * it.Height * 4);
  }
  o.close();
}

Material& FindMat(std::vector<Reference>& rt, std::vector<Material>& mt,
                  const std::string& name) {
  ObjID id;
  for (auto& it : rt) {
    if (it.ID.GetType() != TypeID_Material) {
      continue;
    }
    if (it.Name == name) {
      id = it.ID;
      break;
    }
  }
  for (auto& it : mt) {
    if (it.MaterialID == id) {
      return it;
    }
  }
  throw std::runtime_error("Str Material not found! Search Str: " + name);
}
}  // namespace NMDL