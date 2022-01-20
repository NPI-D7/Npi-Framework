#include <citro3d.h>
#include <3ds.h>
#include "Shader.hpp"
typedef struct
{
	float pos[3];
    float texcoord[2];
    float ptcoord[2];
	u32 color;
} RN2D_Vtx;

class RN2D
{
    public:
    RN2D();
    ~RN2D();
    void prepare();

    private:
    std::unique_ptr<Npi::Shader> _2d;
}
