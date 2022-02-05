#pragma once
#include <3ds.h>
#include <citro3d.h>
#include "color.hpp"

namespace Npi {
   
    class Display {
    public:
        Display(int t_width, int t_height, gfxScreen_t screen, gfx3dSide_t side, GPU_COLORBUF cbuf, C3D_DEPTHTYPE type, u32 flags);
        C3D_RenderTarget* getRenderTarget();
        int getWidth();
        int getHeight();
        void setClearColor(u32 t_color);
        void clear();
        u32 getClearColor();

    private:
        /* data */
        int m_width, m_height;
        u32 m_clearColor;
        gfxScreen_t m_screen;
        
        gfx3dSide_t m_side;
        GPU_COLORBUF m_cbuf;
        C3D_DEPTHTYPE m_type;
        u32 m_flags;

        C3D_RenderTarget* m_target;

    };
} /* d7gfx */