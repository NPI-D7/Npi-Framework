#pragma once
#include <citro3d.h>
#include <3ds.h>
namespace Npi {
    
    class RenderContext {
    public:
       
        enum class ScreenTarget {
            Top,     ///< Top screen
            Bottom   ///< Bottom/touch screen
        };

        enum class Stereo3dSide {
            Left, ///< Left side
            Right ///< Right side
        };

        enum class Mode {
            _2D,   ///< 2D
            _3D ///< 3D
        };

        
        RenderContext(
            int t_modelUniform,
            bool t_3dEnabled,
            Npi::RenderContext::Mode t_mode,
            Npi::RenderContext::Stereo3dSide t_side,
            Npi::RenderContext::ScreenTarget t_target,
            C3D_Mtx& t_model,
            C3D_LightEnv& t_lightEnv,
            C3D_Light& t_light,
            C3D_LightLut& t_lightLut
        );

        int getModelUniform();

        bool is3dEnabled();

        Npi::RenderContext::Mode getMode();

        Npi::RenderContext::Stereo3dSide getSide();

        Npi::RenderContext::ScreenTarget getScreenTarget();

        C3D_Mtx& getModelMatrix();

        C3D_LightEnv& getLightEnvironment();

        C3D_Light& getLight();

        C3D_LightLut& getLightLut();

        void enableTextures(bool t_enable);

    private:
        /* data */
        int m_modelUniformLocation;
        bool m_3dEnabled;
        Npi::RenderContext::Mode m_mode;
        Npi::RenderContext::Stereo3dSide m_side;
        Npi::RenderContext::ScreenTarget m_target;
        C3D_Mtx &m_model;
        C3D_LightEnv& m_lightEnv;
        C3D_Light& m_light;
        C3D_LightLut& m_lightLut;
    };
} /* Npi */