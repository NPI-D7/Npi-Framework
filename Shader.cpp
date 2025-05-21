#include "Shader.hpp"

namespace Npi {
Shader::Shader(std::string path, Type type) {
  std::vector<char> filebuffer;
  u8 f_data;
  u32 f_size;
  std::ifstream file(path.c_str(), std::ios_base::binary);
  if (file) {
    file.seekg(0, std::ios_base::end);
    std::streamsize size = file.tellg();
    if (size > 0) {
      file.seekg(0, std::ios_base::beg);
      filebuffer.resize(static_cast<std::size_t>(size));
      file.read(&filebuffer[0], size);
    }
    filebuffer.push_back('\0');
  } else {
    printf("Err: ShaderNot loadet");
  }
  f_data = (u8)filebuffer[0];
  m_dvlb = DVLB_ParseFile((u32 *)f_data, f_size);
  if (R_FAILED(shaderProgramInit(&m_shader)))
    printf("Err: Shader not loadet");
  if (type == Npi::Shader::Type::VERTEX)

    shaderProgramSetVsh(&m_shader, &m_dvlb->DVLE[0]);
  else {
    shaderProgramSetGsh(&m_shader, &m_dvlb->DVLE[0], 0);
  }
}
Shader::Shader(const u8 *tt_data, u32 tt_size, Type type) {
  m_dvlb = DVLB_ParseFile((u32 *)tt_data, tt_size);
  if (R_FAILED(shaderProgramInit(&m_shader)))
    printf("Err: Shader not loadet");
  if (type == Npi::Shader::Type::VERTEX)

    shaderProgramSetVsh(&m_shader, &m_dvlb->DVLE[0]);
  else {
    shaderProgramSetGsh(&m_shader, &m_dvlb->DVLE[0], 0);
  }
}
Shader::~Shader() {
  shaderProgramFree(&m_shader);
  if (m_dvlb)
    DVLB_Free(m_dvlb);
}
void Shader::AddAttrLoader(int regId, GPU_FORMATS format, int count) {
  Shader::AttrInfo_ attri{regId, format, count};
  this->attrinfo.push_back(attri);
}
void Shader::Load() {
  // m_projectionUniform =
  // shaderInstanceGetUniformLocation(m_shader.vertexShader, "projection");
  // m_modelUniform = shaderInstanceGetUniformLocation(m_shader.vertexShader,
  // "model"); m_viewUniform =
  // shaderInstanceGetUniformLocation(m_shader.vertexShader, "view");
  AttrInfo_Init(&m_attributeInfo);
  for (int i = 0; i < (int)attrinfo.size(); i++) {
    AttrInfo_AddLoader(&m_attributeInfo, attrinfo[i].regId, attrinfo[i].format,
                       attrinfo[i].count);
  }
}
C3D_AttrInfo *Shader::GetAttrInfo() { return &this->m_attributeInfo; }
void Shader::AddUniformLoader(std::string unfname) {
  int smp =
      shaderInstanceGetUniformLocation(m_shader.vertexShader, unfname.c_str());
  Uniform sample{smp, unfname};
  unifo.push_back(sample);
}
int Shader::GetUniform(std::string unfname) {
  int res = 0;
  for (int i = 0; i < (int)unifo.size(); i++) {
    if (unifo[i].name == unfname) {
      res = unifo[i].uniformid;
    }
  }
  return res;
}
DVLB_s *Shader::GetDVLB() { return this->m_dvlb; }
shaderProgram_s *Shader::GetShader() { return &this->m_shader; }
} // namespace Npi