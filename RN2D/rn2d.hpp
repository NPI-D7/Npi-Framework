#include <citro3d.h>
#include <3ds.h>
#include "Shader.hpp"
#include "rn2d_shbin.h"
typedef struct
{
	float pos[3];
    float texcoord[2];
    float ptcoord[2];
	u32 color;
} RN2D_Vtx;

class rn2d
{
    public:
    RN2D();
    ~RN2D();
    void prepare();

    private:
    std::unique_ptr<Npi::Shader> rn2dvs;
}
