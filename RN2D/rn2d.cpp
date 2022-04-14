#include "rn2d.hpp"

int uLoc_mdlvMtx, uLoc_projMtx;
static C3D_Mtx rn_projTop, rn_projBot;
rn2d *ictx;
bool rn2d::CheckBufSpace(unsigned idx, unsigned vtx)
{
	size_t free_idx = this->inctx->idxBufSize - this->inctx->idxBufPos;
	size_t free_vtx = this->inctx->vtxBufSize - this->inctx->vtxBufPos;
	return free_idx >= idx && free_vtx >= vtx;
}

static void FrameEndHook(void* unused)
{
	this->IFlush();
	ictx->GetCTX()->vtxBufPos = 0;
	ictx->GetCTX()->idxBufPos = 0;
	ictx->GetCTX()->idxBufLastPos = 0;
}


rn2d::rn2d()
{
    rn2dvs = std::make_unique<Npi::Shader>(rn2d_shbin, rn2d_shbin_size, Npi::Shader::Type::VERTEX);
    rn2dvs->AddUniformLoader("mdlvMtx");
    rn2dvs->AddUniformLoader("projMtx");
    rn2dvs->AddAttrLoader(0, GPU_FLOAT, 3);
    rn2dvs->AddAttrLoader(1, GPU_FLOAT, 2);
    rn2dvs->AddAttrLoader(2, GPU_FLOAT, 2);
    rn2dvs->AddAttrLoader(3, GPU_UNSIGNED_BYTE, 4);
    rn2dvs->Load();
    uLoc_mdlvMtx = rn2dvs->GetUniform("mdlvMtx");
	uLoc_projMtx = rn2dvs->GetUniform("projMtx");

    BufInfo_Init(&this->inctx->bufInfo);
	BufInfo_Add(&this->inctx->bufInfo, this->inctx->vtxBuf, sizeof(RN2D_Vtx), 4, 0x3210);

	// Cache these common projection matrices
	Mtx_OrthoTilt(&rn_projTop, 0.0f, 400.0f, 240.0f, 0.0f, 1.0f, -1.0f, true);
	Mtx_OrthoTilt(&rn_projBot, 0.0f, 320.0f, 240.0f, 0.0f, 1.0f, -1.0f, true);

	// Get uniform locations
	uLoc_mdlvMtx = shaderInstanceGetUniformLocation(rn2dvs->GetShader()->vertexShader, "mdlvMtx");
	uLoc_projMtx = shaderInstanceGetUniformLocation(rn2dvs->GetShader()->vertexShader, "projMtx");

	// Prepare proctex
	C3D_ProcTexInit(&this->inctx->ptBlend, 0, 1);
	C3D_ProcTexClamp(&this->inctx->ptBlend, GPU_PT_CLAMP_TO_EDGE, GPU_PT_CLAMP_TO_EDGE);
	C3D_ProcTexCombiner(&this->inctx->ptBlend, true, GPU_PT_U, GPU_PT_V);
	C3D_ProcTexFilter(&this->inctx->ptBlend, GPU_PT_LINEAR);

	C3D_ProcTexInit(&this->inctx->ptCircle, 0, 1);
	C3D_ProcTexClamp(&this->inctx->ptCircle, GPU_PT_MIRRORED_REPEAT, GPU_PT_MIRRORED_REPEAT);
	C3D_ProcTexCombiner(&this->inctx->ptCircle, true, GPU_PT_SQRT2, GPU_PT_SQRT2);
	C3D_ProcTexFilter(&this->inctx->ptCircle, GPU_PT_LINEAR);

	// Prepare proctex lut
	float data[129];
	int i;
	for (i = 0; i <= 128; i ++)
		data[i] = i/128.0f;
	ProcTexLut_FromArray(&this->inctx->ptBlendLut, data);

	for (i = 0; i <= 128; i ++)
		data[i] = (i >= 127) ? 0 : 1;
	ProcTexLut_FromArray(&this->inctx->ptCircleLut, data);

	this->inctx->flags = RN2DF_Active | (RN2DF_Mode_ImageSolid << (RN2DF_TintMode_Shift-RN2DF_Mode_Shift));
	this->inctx->vtxBufPos = 0;
	this->inctx->idxBufPos = 0;
	this->inctx->idxBufLastPos = 0;
	Mtx_Identity(&this->inctx->projMtx);
	Mtx_Identity(&this->inctx->mdlvMtx);
	this->inctx->fadeClr = 0;

	C3D_FrameEndHook(FrameEndHook, NULL);
}

rn2d::~rn2d()
{
	if (!(inctx->flags & RN2DF_Active))
		return;

	inctx->flags = 0;
	C3D_FrameEndHook(NULL, NULL);
	linearFree(inctx->idxBuf);
	linearFree(inctx->vtxBuf);
}

void rn2d::prepare()
{
	if (!(inctx->flags & RN2DF_Active))
		return;

	inctx->flags  = (inctx->flags &~ (RN2DF_Mode_Mask|RN2DF_ProcTex_Mask)) | RN2DF_DirtyAny;
	inctx->curTex = NULL;

	C3D_BindProgram(rn2dvs->GetShader());
	C3D_SetAttrInfo(rn2dvs->GetAttrInfo());
	C3D_SetBufInfo(&inctx->bufInfo);

	// texenv usage:
	// 0..4: used by switchable mode
	// 5..6: used by post processing
	C3D_TexEnv* env;

	// Configure texenv4 as a no-op (reserved)
	env = C3D_GetTexEnv(4);
	C3D_TexEnvInit(env);

	// Configure texenv5 to apply the fade color
	// texenv5.rgb = mix(texenv4.rgb, fadeclr.rgb, fadeclr.a);
	// texenv5.a   = texenv4.a;
	env = C3D_GetTexEnv(5);
	C3D_TexEnvInit(env);
	C3D_TexEnvSrc(env, C3D_RGB, GPU_PREVIOUS, GPU_CONSTANT, GPU_CONSTANT);
	C3D_TexEnvOpRgb(env, GPU_TEVOP_RGB_SRC_COLOR, GPU_TEVOP_RGB_SRC_COLOR, GPU_TEVOP_RGB_ONE_MINUS_SRC_ALPHA);
	C3D_TexEnvFunc(env, C3D_RGB, GPU_INTERPOLATE);
	C3D_TexEnvColor(env, inctx->fadeClr);

	// Configure depth test to overwrite pixels with the same depth (needed to draw overlapping sprites)
	C3D_DepthTest(true, GPU_GEQUAL, GPU_WRITE_ALL);

	// Don't cull anything
	C3D_CullFace(GPU_CULL_NONE);
}

void rn2d::IFlush()
{
	size_t len = inctx->idxBufPos - inctx->idxBufLastPos;
	if (!len) return;
	C3D_DrawElements(GPU_TRIANGLES, len, C3D_UNSIGNED_SHORT, &inctx->idxBuf[inctx->idxBufLastPos]);
	inctx->idxBufLastPos = inctx->idxBufPos;
}

bool rn2d::Flush()
{
	if (!(inctx->flags & RN2DF_Active))
		return;

	this->IFlush();
}

void rn2d::SceneSize(u32 width, u32 height, bool tilt)
{
	if (!(inctx->flags & RN2DF_Active))
		return;

	if (tilt)
	{
		u32 temp = width;
		width = height;
		height = temp;
	}

	inctx->flags |= RN2DF_DirtyProj;
	inctx->sceneW = width;
	inctx->sceneH = height;

	// Check for cached projection matrices
	if (height == GSP_SCREEN_WIDTH && tilt)
	{
		if (width == GSP_SCREEN_HEIGHT_TOP || width == GSP_SCREEN_HEIGHT_TOP_2X)
		{
			Mtx_Copy(&inctx->projMtx, &rn_projTop);
			return;
		}
		else if (width == GSP_SCREEN_HEIGHT_BOTTOM)
		{
			Mtx_Copy(&inctx->projMtx, &rn_projBot);
			return;
		}
	}

	// Construct the projection matrix
	(tilt ? Mtx_OrthoTilt : Mtx_Ortho)(&inctx->projMtx, 0.0f, width, height, 0.0f, 1.0f, -1.0f, true);
}

void rn2d::ResetView()
{
	if (!(inctx->flags & RN2DF_Active))
		return;

	Mtx_Identity(&inctx->mdlvMtx);
	inctx->flags |= RN2DF_DirtyMdlv;

}

void rn2d::SaveView(C3D_Mtx* matrix)
{
	Mtx_Copy(matrix, &inctx->mdlvMtx);
}

void rn2d::RestoreView(const C3D_Mtx* matrix)
{
	if (!(inctx->flags & RN2DF_Active))
		return;

	Mtx_Copy(&inctx->mdlvMtx, matrix);
	inctx->flags |= RN2DF_DirtyMdlv;
}

void rn2d::InitDisplays()
{
	D7Top = std::make_unique<Npi::Display>(!gfxIsWide() ? 400 : 800, 320, GFX_TOP, GFXLEFT, GPU_RGB8, GPU_RB_DEPTH16, GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) |
			GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) |
			GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO));
	D7Bottom = std::make_unique<Npi::Display>(400, 320, GFX_BOTTOM, GFXLEFT, GPU_RGB8, GPU_RB_DEPTH16, GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) |
			GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) |
			GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO));
	D7TopRight = std::make_unique<Npi::Display>(!gfxIsWide() ? 400 : 800, 320, GFX_TOP, GFXRIGHT, GPU_RGB8, GPU_RB_DEPTH16, GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) |
			GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) |
			GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO));

}

void rn2d::ClearDisplay(Npi::Display *display, u32 color)
{
	this->IFlush();
	C3D_FrameSplit(0);
	C3D_RenderTargetClear(display->getRenderTarget(), C3D_CLEAR_ALL, __builtin_bswap32(color), 0);
}

void rn2d::Fade(u32 color)
{
	if (!(inctx->flags & RN2DF_Active))
		return false;

	inctx->flags |= RN2DF_DirtyFade;
	inctx->fadeClr = color;
}

void rn2d::SetTintMode(C2D_TintMode mode)
{
	C2Di_Context* ctx = C2Di_GetContext();
	if (!(ctx->flags & C2DiF_Active))
		return false;

	u32 new_mode;
	switch (mode)
	{
		default:
		case C2D_TintSolid:
			new_mode = C2DiF_Mode_ImageSolid;
			break;
		case C2D_TintMult:
			new_mode = C2DiF_Mode_ImageMult;
			break;
		case C2D_TintLuma:
			new_mode = C2DiF_Mode_ImageLuma;
			break;
	}

	ctx->flags = (ctx->flags &~ C2DiF_TintMode_Mask) | (new_mode << (C2DiF_TintMode_Shift - C2DiF_Mode_Shift));
}