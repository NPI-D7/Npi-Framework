#pragma once
#include <citro3d.h>
#include "RenderContext.hpp"

namespace Npi {
   
    class RObject {
    public:
        virtual void draw(Npi::RenderContext t_context) = 0;
    };
}
