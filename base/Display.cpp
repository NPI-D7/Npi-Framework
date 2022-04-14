#include "Display.hpp"

namespace Npi {
    Display::Display(int t_width, int t_height, gfxScreen_t screen, gfx3dSide_t side, GPU_COLORBUF cbuf, C3D_DEPTHTYPE type, u32 flags) :
        m_width(t_width),
        m_height(t_height),
        m_clearColor(Npi::Color::rgba8(0, 0, 0, 255)),
        m_cbuf(cbuf),
        m_screen(screen),
        m_side(side),
        m_type(type),
        m_flags(flags)
        {
            m_target = C3D_RenderTargetCreate(t_height, t_width, cbuf, type);
            if (m_target)
		C3D_RenderTargetSetOutput(m_target, screen, side, flags);
    }

    C3D_RenderTarget* RenderTarget::getRenderTarget() {
        return m_target;
    }

    int Display::getWidth() {
        return m_width;
    }

    int Display::getHeight() {
        return m_height;
    }

    void Display::setClearColor(u32 t_color) {
        m_clearColor = t_color;
    }

    void Display::clear() {
        C3D_RenderTargetClear(m_target, C3D_CLEAR_ALL, __builtin_bswap32(m_clearColor), 0);
    }

    u32 Display::getClearColor() {
        return m_clearColor;
    }
} /* Npi */
