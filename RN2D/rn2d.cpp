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
	//C2Di_FlushVtxBuf();
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
