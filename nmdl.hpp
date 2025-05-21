#pragma once

#include <pd.hpp>

namespace NMDL {
enum TypeID : PD::u8 {
  TypeID_Mesh,
  TypeID_Material,
  TypeID_Tex,
};

/** Struct to Split 32 bit ID into type and objID */
struct ObjID {
  constexpr ObjID() : ID(0) {}
  /** Note that this ID only supports a 24Bit maximum */
  constexpr ObjID(TypeID t, PD::u32 obj) : ID(t | (obj << 8)) {}
  constexpr ObjID(PD::u32 id) : ID(id) {}

  PD::u32 GetID() { return ID >> 8; }
  TypeID GetType() { return (TypeID)(ID & 0xff); }

  bool operator==(const ObjID& b) const { return ID == b.ID; }

  PD::u32 ID;
};

struct Vertex {
  fvec3 Pos;
  fvec2 UV;
  fvec3 Normal;
};

struct Material {
  ObjID MaterialID;
  fvec3 AmbientColor;
  fvec3 DiffuseColor;
  fvec3 SpecularColor;
  float Specular;
  float Density;
  float Dissolve;
  int Illumination;
  ObjID AmbientTex;
  ObjID DiffuseTex;
  ObjID SpecularTex;
  ObjID SpecHighlightTex;
  ObjID AlphaTex;
  ObjID NormalMap;
};

/**
 * Note that every std::vector could be a Simple pointer reference as
 * we set the num of elements in front of the elements
 */

struct Mesh {
  ObjID MeshID = 0;
  PD::u32 NumVertices;
  std::vector<Vertex> Vertices;
  PD::u32 NumIndices;
  std::vector<PD::u32> Indices;
  ObjID MaterialID = 0;
};

struct Texture {
  ObjID ID;
  PD::u32 Width;
  PD::u32 Height;
  /** Buffer Len is Width * Height * 4 */
  std::vector<PD::u8> Buffer;
};

struct Reference {
  ObjID ID;
  /**
   * Note that start pos needs to be the same ObjID
   * as well as it is relative to the end of the reference table
   */
  PD::u32 StartPos;
  PD::u32 NameLen;
  std::string Name;
};

struct Model {
  PD::u32 Magic;
  PD::u32 Version;
  PD::u32 DataBeg;
  PD::u32 ReferenceTabLen;
  std::vector<Reference> ReferenceTab;
  /** Data */
  PD::u32 NumMeshes;
  std::vector<Mesh> Meshes;
  PD::u32 NumMaterials;
  std::vector<Material> Materials;
  PD::u32 NumTextures;
  std::vector<Texture> Textures;
};

Material& FindMat(std::vector<Reference>& rt, std::vector<Material>& mt,
                  const std::string& name);
void Load(Model& mdl, const std::string& filename);
void Save(Model& mdl, const std::string& filename);
}  // namespace NMDL