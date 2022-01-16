#pragma once
#include <3ds.h>
#include <citro3d.h>
#include "color.hpp"

namespace Npi {
   
    class RenderTarget {
    public:

        RenderTarget(int t_width, int t_height);

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
        C3D_RenderTarget* m_target;

    };
} /* d7gfx */