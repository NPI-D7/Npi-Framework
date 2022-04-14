#include <citro3d.h>
#include <3ds.h>
#include "Shader.hpp"
#include "rn2d_shbin.h"
#include "Display.hpp"
#include <memory>

typedef struct
{
	float pos[3];
    float texcoord[2];
    float ptcoord[2];
	u32 color;
} RN2D_Vtx;

enum
{
	RN2DF_Active       = BIT(0),
	RN2DF_DirtyProj    = BIT(1),
	RN2DF_DirtyMdlv    = BIT(2),
	RN2DF_DirtyTex     = BIT(3),
	RN2DF_DirtyMode    = BIT(4),
	RN2DF_DirtyFade    = BIT(5),

	RN2DF_Mode_Shift      = 8,
	RN2DF_Mode_Mask       = 0xf << RN2DF_Mode_Shift,
	RN2DF_Mode_Solid      = 0   << RN2DF_Mode_Shift,
	RN2DF_Mode_Circle     = 1   << RN2DF_Mode_Shift,
	RN2DF_Mode_Text       = 2   << RN2DF_Mode_Shift,
	RN2DF_Mode_ImageSolid = 3   << RN2DF_Mode_Shift,
	RN2DF_Mode_ImageMult  = 4   << RN2DF_Mode_Shift,
	RN2DF_Mode_ImageLuma  = 5   << RN2DF_Mode_Shift,

	RN2DF_ProcTex_Shift  = 12,
	RN2DF_ProcTex_Mask   = 0xf << RN2DF_ProcTex_Shift,
	RN2DF_ProcTex_None   = 0   << RN2DF_ProcTex_Shift,
	RN2DF_ProcTex_Blend  = 1   << RN2DF_ProcTex_Shift,
	RN2DF_ProcTex_Circle = 2   << RN2DF_ProcTex_Shift,

	RN2DF_TintMode_Shift = 16,
	RN2DF_TintMode_Mask  = 0xf << RN2DF_TintMode_Shift,

	RN2DF_DirtyAny = RN2DF_DirtyProj | RN2DF_DirtyMdlv | RN2DF_DirtyTex | RN2DF_DirtyMode | RN2DF_DirtyFade,
};

struct CTX{
    C3D_BufInfo bufInfo;
	C3D_ProcTex ptBlend;
	C3D_ProcTex ptCircle;
	C3D_ProcTexLut ptBlendLut;
	C3D_ProcTexLut ptCircleLut;
	u32 sceneW, sceneH;

	RN2D_Vtx* vtxBuf;
	u16* idxBuf;

    

	size_t vtxBufSize;
	size_t vtxBufPos;

	size_t idxBufSize;
	size_t idxBufPos;
	size_t idxBufLastPos;

	u32 flags;
	C3D_Mtx projMtx;
	C3D_Mtx mdlvMtx;
	C3D_Tex* curTex;
	u32 fadeClr;
};

class rn2d
{
    public:
    rn2d();
    ~rn2d();
    void prepare();
	bool Flush();
	void SceneSize(u32 width, u32 height, bool tilt);
    inline CTX* GetCTX(){return inctx;}
	//View
	void ResetView();
	void SaveView(C3D_Mtx* matrix);
	void RestoreView(const C3D_Mtx* matrix);
	void TranlateView(float , float y);
	void RotateView(float delta);
	void ShearView(float x, float y);
	void ScaleView(float x, float y);
	//Display
	void InitDisplays();
	void ClearDisplay(Npi::Display *display, u32 color)
	void Fade(u32 color)
	std::unique_ptr<Npi::Display> D7Top;
	std::unique_ptr<Npi::Display> D7Bottom;
	std::unique_ptr<Npi::Display> D7TopRight;
    private:
	void IFlush();
    bool CheckBufSpace(unsigned idx, unsigned vtx);
	CTX* inctx;
    //////////////////////////////////////////////////////////////////////////////////////////
    std::unique_ptr<Npi::Shader> rn2dvs;
};
