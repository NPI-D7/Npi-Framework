#pragma once
#include <citro3d.h>
#include <3ds.h>

#include <cstring>
#include <functional>
#include <map>
#include <vector>
#include <memory>
#include "rn3d_shbin.h"
#include "Shader.hpp"

#include "RenderContext.hpp"
#include "RenderTarget.hpp"
#include "camera.hpp"
#include "RObject.hpp"
#include "color.hpp"
namespace Npi
{
    class Renderer
    {
        public:
        Renderer(bool _3d = false);
        ~Renderer();
        
        
        void Render(bool d_clear);
        void clear();

        void set3d(bool t_enabled);
        void useCulling(bool t_useCulling);
        bool isUsingCulling();
        void setWide(bool t_wide);
        
        void setClearColor(Npi::Color t_color);
        void setClearColor(Npi::Color t_color, Npi::RenderContext::ScreenTarget t_target);
        Npi::Color getClearColor(Npi::RenderContext::ScreenTarget t_target);
        void drawTop(Npi::RObject& t_object, Npi::RenderContext::Mode t_mode = Npi::RenderContext::Mode::_2D, int t_layer = 0);
        void drawTop(Npi::RObject& t_object, std::function<bool()> t_shadingFunction, Npi::RenderContext::Mode t_mode = Npi::RenderContext::Mode::_2D, int t_layer = 0);
        void drawBottom(Npi::RObject& t_object, Npi::RenderContext::Mode t_mode = Npi::RenderContext::Mode::_2D, int t_layer = 0);
        void drawBottom(Npi::RObject& t_object, std::function<bool()> t_shadingFunction, Npi::RenderContext::Mode t_mode = Npi::RenderContext::Mode::_2D, int t_layer = 0);
        static int getScreenWidth(Npi::RenderContext::ScreenTarget t_target);
        static int getScreenHeight();

        void setCamera(Npi::Camera t_camera, Npi::RenderContext::ScreenTarget t_target);
        Npi::Camera& getCamera(Npi::RenderContext::ScreenTarget t_target);
        void useFog(bool t_useFog, Npi::RenderContext::ScreenTarget t_target);
        bool getUseFog(Npi::RenderContext::ScreenTarget t_target);
        void setFogDensity(float t_density,Npi::RenderContext::ScreenTarget t_target);
        float getFogDensity(Npi::RenderContext::ScreenTarget t_target);
        private:
        /*Data*/

        
        void prepare();
        void prepareFog(Npi::RenderContext::ScreenTarget t_target);
        void prepareLights(Npi::RenderContext::ScreenTarget t_target);

        int m_projectionUniform, m_modelUniform, m_viewUniform;
        bool m_3dEnabled, m_useCulling;
        Npi::Color m_clearColorTop, m_clearColorBottom;
        Npi::RenderTarget *m_targetTopLeft, *m_targetTopRight, *m_targetBottom;
        Npi::Camera &m_cameraTop, &m_cameraBottom;

        std::map<int, std::vector<std::pair<Npi::RObject*, std::function<bool()>>>, std::less<int>> m_drawStackTop2d, m_drawStackTop3d, m_drawStackBottom2d, m_drawStackBottom3d;
        // shader
        /*DVLB_s* m_dvlb;
        shaderProgram_s m_shader;
        */
        std::unique_ptr<Npi::Shader> rn3d;
       
        /*
        // attribute information
        C3D_AttrInfo m_attributeInfo;
        */
        // matrices
        C3D_Mtx m_projection, m_model, m_view;

        //light
        C3D_LightEnv m_lightEnvTop, m_lightEnvBottom;
        C3D_Light m_lightTop, m_lightBottom;
        C3D_LightLut m_lutPhongTop, m_lutPhongBottom;

        // fog
        bool m_useFogTop, m_useFogBottom;
        float m_fogDensityTop, m_fogDensityBottom;
        C3D_FogLut m_fogLutTop, m_fogLutBottom;
        //Skybox
        
        //Widemode
        bool m_wide = false;
        
    };
}
