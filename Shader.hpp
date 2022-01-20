#pragma once
#include <3ds.h>
#include <citro3d.h>
#include <vector>
#include <string>
#include <fstream>
namespace Npi
{
    class Shader
    {
        
        
        public:
        enum Type
        {
            VERTEX, //Vertex Shader
            GEOMETRY,  //Geometry shader (Not Supported)
        };
        Shader(std::string path, Type type);
        Shader(const u8* tt_data, u32 tt_size, Type type);
        ~Shader();
        void AddAttrLoader(int regId, GPU_FORMATS format, int count);
        void AddUniformLoader(std::string unfname);
        void Load();
        C3D_AttrInfo* GetAttrInfo();
        int GetUniform(std::string unifname);
        DVLB_s* GetDVLB();
        shaderProgram_s* GetShader();
        private:
        //
        struct AttrInfo_
        {
            int regId;
            GPU_FORMATS format;
            int count;
        };
        struct Uniform
        {
            int uniformid;
            std::string name;
        };
        //
        
        //
        DVLB_s* m_dvlb;
        shaderProgram_s m_shader;
        std::vector<Uniform> unifo;
        //int m_projectionUniform, m_modelUniform, m_viewUniform;
        // attribute information
        std::vector<AttrInfo_> attrinfo;
        C3D_AttrInfo m_attributeInfo;
    };
}
