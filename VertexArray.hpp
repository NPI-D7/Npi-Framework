#pragma once
#include <3ds.h>
#include <citro3d.h>
#include "vertex.hpp"
#include "polygon.hpp"
#include "RObject.hpp"

namespace Npi
{

    class VertexArray : public Npi::RObject
    {
    public:
        static VertexArray* Create();
        static void Dispose(VertexArray*);
        void AddVertex(const Npi::Polygon::Vertex& vertex);
        Npi::Polygon::Vertex& GetVertex(u32 index);
        void Complete();
        u32 Size();
        void draw(Npi::RenderContext t_context);

    private:
        VertexArray(u32 startInd) : startIndex(startInd), size(0) {};
        ~VertexArray() {};
        u32 startIndex;
        u32 size;
        static bool lockCreate;
        static u32 currIndex;

        static constexpr size_t MAX_VERTEX_COUNT = 30000;

        static DVLB_s* vshader_dvlb;
        static shaderProgram_s program;
        static int uLoc_projection, uLoc_modelView;
        static C3D_MtxStack projStack;
        static C3D_MtxStack modelStack;
        static Npi::Polygon::Vertex* vbo_data;
    };
} // namespace Npi