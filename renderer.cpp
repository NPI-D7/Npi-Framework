#include "renderer.hpp"

Npi::Camera _1, _2;

namespace Npi
{
    Renderer::Renderer(bool _3d) :
            m_3dEnabled(_3d),
            m_useCulling(true),
            m_clearColorTop(Npi::Color(0, 0, 0)),
            m_clearColorBottom(Npi::Color(0, 0, 0)),
            m_cameraTop(_1),
            m_cameraBottom(_2),
            m_useFogTop(false),
            m_useFogBottom(false),
            m_fogDensityTop(0.05),
            m_fogDensityBottom(0.05)
    {
        //C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
        m_targetTopLeft  = new Npi::RenderTarget(400, 240);
        m_targetTopRight = new Npi::RenderTarget(400, 240);
        m_targetBottom   = new Npi::RenderTarget(320, 240);

        u32 flags = GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) | GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO);

        C3D_RenderTargetSetOutput(m_targetTopLeft->getRenderTarget(),  GFX_TOP,    GFX_LEFT,  flags);
        C3D_RenderTargetSetOutput(m_targetTopRight->getRenderTarget(), GFX_TOP,    GFX_RIGHT, flags);
        C3D_RenderTargetSetOutput(m_targetBottom->getRenderTarget(),   GFX_BOTTOM, GFX_LEFT,  flags);

        if (_3d)
        {
            set3d(_3d);
        }
        //consoleInit(GFX_TOP, NULL);
        printf("\nShader: Init");
        //Npi::Shader standart(vshader_shbin, vshader_shbin_size, Npi::Shader::Type::VERTEX);
        rn3d = std::make_unique<Npi::Shader>(rn3d_shbin, rn3d_shbin_size, Npi::Shader::Type::VERTEX);
        
        printf("\nShader: AddUniforms");
        rn3d->AddUniformLoader("projection");
        rn3d->AddUniformLoader("model");
        rn3d->AddUniformLoader("view");
        printf("\nShader: AddAttributes");
        rn3d->AddAttrLoader(0, GPU_FLOAT, 3);
        rn3d->AddAttrLoader(1, GPU_FLOAT, 2);
        rn3d->AddAttrLoader(2, GPU_FLOAT, 3);
        rn3d->AddAttrLoader(3, GPU_FLOAT, 4);
        printf("\nShader: GetUniforms");
        this->m_projectionUniform = rn3d->GetUniform("projection");
        this->m_modelUniform = rn3d->GetUniform("model");
        this->m_viewUniform = rn3d->GetUniform("view");
        printf("\nShader: Load");
        rn3d->Load();
        printf("\nShader: GetAttrInfo");
        //this->m_attributeInfo = standart.GetAttrInfo();
        /*//shader
        m_dvlb = DVLB_ParseFile((u32*) vshader_shbin, vshader_shbin_size);
        shaderProgramInit(&m_shader);
        shaderProgramSetVsh(&m_shader, &m_dvlb->DVLE[0]);

        // get location of uniforms used in the vertex shader.
        m_projectionUniform = shaderInstanceGetUniformLocation(m_shader.vertexShader, "projection");
        m_modelUniform = shaderInstanceGetUniformLocation(m_shader.vertexShader, "model");
        m_viewUniform = shaderInstanceGetUniformLocation(m_shader.vertexShader, "view");

        AttrInfo_Init(&m_attributeInfo);
        AttrInfo_AddLoader(&m_attributeInfo, 0, GPU_FLOAT, 3); // v0=position
        AttrInfo_AddLoader(&m_attributeInfo, 1, GPU_FLOAT, 2); // v1=texcoord
        AttrInfo_AddLoader(&m_attributeInfo, 2, GPU_FLOAT, 3); // v2=normal
        AttrInfo_AddLoader(&m_attributeInfo, 3, GPU_FLOAT, 4); // v3=color
        */
        //light
        printf("\nLight: Create");
        C3D_FVec lightVec;
        printf("\nLight: Init Top");
        C3D_LightEnvInit(&m_lightEnvTop);
        LightLut_Phong(&m_lutPhongTop, 30);
        C3D_LightEnvLut(&m_lightEnvTop, GPU_LUT_D0, GPU_LUTINPUT_LN, false, &m_lutPhongTop);
        lightVec = FVec4_New(0.0f, 0.0f, -0.5f, 1.0f);
        printf("\nLight: Set Top");
        C3D_LightInit(&m_lightTop, &m_lightEnvTop);
        C3D_LightColor(&m_lightTop, 1.0, 1.0, 1.0);
        C3D_LightPosition(&m_lightTop, &lightVec);
        printf("\nLight: Init Bottom");
        C3D_LightEnvInit(&m_lightEnvBottom);
        LightLut_Phong(&m_lutPhongBottom, 30);
        C3D_LightEnvLut(&m_lightEnvBottom, GPU_LUT_D0, GPU_LUTINPUT_LN, false, &m_lutPhongBottom);
        lightVec = FVec4_New(0.0f, 0.0f, -0.5f, 1.0f);
        printf("\nLight: Set Bottom");
        C3D_LightInit(&m_lightBottom, &m_lightEnvBottom);
        C3D_LightColor(&m_lightBottom, 1.0, 1.0, 1.0);
        C3D_LightPosition(&m_lightBottom, &lightVec);
        printf("\nRenderer: Clear");
        clear();
    }
    Renderer::~Renderer()
    {
        //C2D_Fini();robj
        //shaderProgramFree(&m_shader);
        //DVLB_Free(m_dvlb);
        delete m_targetTopLeft;
        delete m_targetTopRight;
        delete m_targetBottom;
    }
    void Renderer::Render(bool d_clear)
    {
        printf("\nRenderer: Start Drawing");
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        printf("\nRenderer: Clear");
        if (d_clear)
        {
            m_targetTopLeft->clear();
            m_targetTopRight->clear();
            m_targetBottom->clear();
            clear();
        }
        printf("\nRenderer: Stack 3d");
        if(m_drawStackTop3d.size() > 0 || m_drawStackBottom3d.size() > 0) {
            prepare();
            C3D_DepthTest(true, GPU_GEQUAL, GPU_WRITE_ALL);
            Mtx_PerspStereoTilt(&m_projection, C3D_AngleFromDegrees(40.0f), C3D_AspectRatioBot, 0.01f, 1000.0f, 0, 2.0f, false);

            C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, m_projectionUniform, &m_projection);
            C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, m_viewUniform, &m_cameraBottom.getViewMatrix());

            if(m_drawStackBottom3d.size() > 0) {
                C3D_FrameDrawOn(m_targetBottom->getRenderTarget());
                prepareLights(Npi::RenderContext::ScreenTarget::Bottom);
                if (m_useFogBottom) prepareFog(Npi::RenderContext::ScreenTarget::Bottom);

                for(const auto &entry : m_drawStackBottom3d) { // for every layer
                    for(const auto &robj : entry.second) { // draw every object
                        if (robj.second()) {
                            robj.first->draw(Npi::RenderContext(
                                m_modelUniform, // modelUniform
                                m_3dEnabled, // 3dEnabled
                                Npi::RenderContext::Mode::_3D,        // mode
                                Npi::RenderContext::Stereo3dSide::Left,   // side
                                Npi::RenderContext::ScreenTarget::Bottom, // target
                                m_model,  // model
                                m_lightEnvBottom, // lightEnv
                                m_lightBottom,    // light
                                m_lutPhongBottom  // lutPhong
                            ));
                        }
                    }
                }

                m_drawStackBottom3d.clear();
            }
            printf("\nRenderer: Stack 3D");
            if (m_drawStackTop3d.size() > 0) {
                C3D_FrameDrawOn(m_targetTopLeft->getRenderTarget());
                prepareLights(Npi::RenderContext::ScreenTarget::Top);
                if (m_useFogTop) prepareFog(Npi::RenderContext::ScreenTarget::Top);

                // tilt stereo perspective
                if (m_3dEnabled) {
                    Mtx_PerspStereoTilt(&m_projection, C3D_AngleFromDegrees(40.0f), C3D_AspectRatioTop, 0.01f, 1000.0f, -(osGet3DSliderState() / 3.0f), 2.0f, false);
                } else {
                    Mtx_PerspStereoTilt(&m_projection, C3D_AngleFromDegrees(40.0f), C3D_AspectRatioTop, 0.01f, 1000.0f, 0, 2.0f, false);
                }

                C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, m_projectionUniform, &m_projection);
                C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, m_viewUniform, &m_cameraTop.getViewMatrix());

                for (const auto &entry : m_drawStackTop3d) { // for every layer
                    for (const auto &robj : entry.second) { // draw every object
                        if (robj.second()) {
                            robj.first->draw(Npi::RenderContext(
                                m_modelUniform, // modelUniform
                                m_3dEnabled, // 3dEnabled
                                Npi::RenderContext::Mode::_3D,      // mode
                                Npi::RenderContext::Stereo3dSide::Left, // side
                                Npi::RenderContext::ScreenTarget::Top,  // target
                                m_model,  // model
                                m_lightEnvTop, // lightEnv
                                m_lightTop,    // light
                                m_lutPhongTop  // lutPhong
                            ));
                        }
                    }
                }
                printf("\nRenderer: Fixed 3d");
                if (m_3dEnabled && osGet3DSliderState() > 0.0f) {
                    C3D_FrameDrawOn(m_targetTopRight->getRenderTarget());

                    // tilt stereo perspective
                    if (m_3dEnabled) {
                        Mtx_PerspStereoTilt(&m_projection, C3D_AngleFromDegrees(40.0f), C3D_AspectRatioTop, 0.01f, 1000.0f, osGet3DSliderState() / 3.0f, 2.0f, false);
                    } else {
                        Mtx_PerspStereoTilt(&m_projection, C3D_AngleFromDegrees(40.0f), C3D_AspectRatioTop, 0.01f, 1000.0f, 0, 2.0f, false);
                    }

                    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, m_projectionUniform, &m_projection);

                    for (const auto &entry : m_drawStackTop3d) { // for every layer
                        for (const auto &robj : entry.second) { // draw every object
                            if (robj.second()) {
                                robj.first->draw(Npi::RenderContext(
                                    m_modelUniform, // modelUniform
                                    m_3dEnabled, // 3dEnabled
                                    Npi::RenderContext::Mode::_3D,       // mode
                                    Npi::RenderContext::Stereo3dSide::Right, // side
                                    Npi::RenderContext::ScreenTarget::Top,   // target
                                    m_model,  // model
                                    m_lightEnvTop, // lightEnv
                                    m_lightTop,    // light
                                    m_lutPhongTop  // lutPhong
                                ));
                            }
                        }
                    }
                }

                m_drawStackTop3d.clear();
            }
        }
        printf("\nRenderer: Stack 2D");
        // draw 2d
        if (m_drawStackTop2d.size() > 0 || m_drawStackBottom2d.size() > 0) {
            //C2D_Prepare();
            C3D_DepthTest(true, GPU_ALWAYS, GPU_WRITE_ALL);

            if(m_drawStackBottom2d.size() > 0) {
               // C2D_SceneBegin(m_targetBottom->getRenderTarget());

                for(const auto &entry : m_drawStackBottom2d) { // for every layer
                    for(const auto &robj : entry.second) { // draw every object
                        if (robj.second()) {
                            robj.first->draw(Npi::RenderContext(
                                m_modelUniform, // modelUniform
                                m_3dEnabled, // 3dEnabled
                                Npi::RenderContext::Mode::_2D,           // mode
                                Npi::RenderContext::Stereo3dSide::Left,   // side
                                Npi::RenderContext::ScreenTarget::Bottom, // target
                                m_model,  // model
                                m_lightEnvBottom, // lightEnv
                                m_lightBottom,    // light
                                m_lutPhongBottom  // lutPhong
                            ));
                        }
                    }
                }

                m_drawStackBottom2d.clear();
            }

            if(m_drawStackTop2d.size() > 0) {
               // C2D_SceneBegin(m_targetTopLeft->getRenderTarget());

                for(const auto &entry : m_drawStackTop2d) { // for every layer
                    for(const auto &robj : entry.second) { // draw every object
                        if (robj.second()) {
                            robj.first->draw(Npi::RenderContext(
                                m_modelUniform, // modelUniform
                                m_3dEnabled, // 3dEnabled
                                Npi::RenderContext::Mode::_2D,         // mode
                                Npi::RenderContext::Stereo3dSide::Left, // side
                                Npi::RenderContext::ScreenTarget::Top,  // target
                                m_model,  // model
                                m_lightEnvTop, // lightEnv
                                m_lightTop,    // light
                                m_lutPhongTop  // lutPhong
                            ));
                        }
                    }
                }

                if(m_3dEnabled && osGet3DSliderState() > 0.0f) {
                    //C2D_SceneBegin(m_targetTopRight->getRenderTarget());

                    for(const auto &entry : m_drawStackTop2d) { // for every layer
                        for(const auto &robj : entry.second) { // draw every object
                            if (robj.second()) {
                                robj.first->draw(Npi::RenderContext(
                                    m_modelUniform, // modelUniform
                                    m_3dEnabled, // 3dEnabled
                                    Npi::RenderContext::Mode::_2D,          // mode
                                    Npi::RenderContext::Stereo3dSide::Right, // side
                                    Npi::RenderContext::ScreenTarget::Top,   // target
                                    m_model,  // model
                                    m_lightEnvTop, // lightEnv
                                    m_lightTop,    // light
                                    m_lutPhongTop  // lutPhong
                                ));
                            }
                        }
                    }
                }

                m_drawStackTop2d.clear();
            }
        }
        printf("\nRenderer: End Drawing");
        C3D_FrameEnd(0);
    }

    void Renderer::clear() {
        C3D_RenderTargetClear(m_targetTopLeft->getRenderTarget(), C3D_CLEAR_ALL, __builtin_bswap32(m_clearColorTop.getRgba8()), 0);
        C3D_RenderTargetClear(m_targetTopRight->getRenderTarget(), C3D_CLEAR_ALL, __builtin_bswap32(m_clearColorTop.getRgba8()), 0);
        C3D_RenderTargetClear(m_targetBottom->getRenderTarget(), C3D_CLEAR_ALL, __builtin_bswap32(m_clearColorBottom.getRgba8()), 0);
    }

    void Renderer::set3d(bool t_enabled) {
        gfxSet3D(t_enabled);
        m_3dEnabled = t_enabled;
    }

    void Renderer::useCulling(bool t_useCulling) {
        m_useCulling = t_useCulling;
    }

    bool Renderer::isUsingCulling() {
        return m_useCulling;
    }

    void Renderer::setClearColor(Npi::Color t_color) {
        m_clearColorTop = t_color;
        m_clearColorBottom = t_color;
    }

    void Renderer::setClearColor(Npi::Color t_color, Npi::RenderContext::ScreenTarget t_target) {
        t_target == Npi::RenderContext::ScreenTarget::Top ?
                    m_clearColorTop = t_color :
                    m_clearColorBottom = t_color;
    }

    Npi::Color Renderer::getClearColor(Npi::RenderContext::ScreenTarget t_target) {
        return t_target == Npi::RenderContext::ScreenTarget::Top ? m_clearColorTop : m_clearColorBottom;
    }

    void Renderer::drawTop(Npi::RObject& t_object, Npi::RenderContext::Mode t_mode, int t_layer) {
        if (t_mode == Npi::RenderContext::Mode::_2D) {
            if(m_drawStackTop2d.count(t_layer) > 0) {
                m_drawStackTop2d[t_layer].insert(m_drawStackTop2d[t_layer].end(), std::make_pair(&t_object, [](){return true;}));
            } else {
                std::vector<std::pair<Npi::RObject*, std::function<bool()>>> newStack;
                newStack.push_back(std::make_pair(&t_object, [](){return true;}));
                m_drawStackTop2d.insert(std::make_pair(t_layer, newStack));
            }
        } else {
            if(m_drawStackTop3d.count(t_layer) > 0) {
                m_drawStackTop3d[t_layer].insert(m_drawStackTop3d[t_layer].end(), std::make_pair(&t_object, [](){return true;}));
            } else {
                std::vector<std::pair<Npi::RObject*, std::function<bool()>>> newStack;
                newStack.push_back(std::make_pair(&t_object, [](){return true;}));
                m_drawStackTop3d.insert(std::make_pair(t_layer, newStack));
            }
        }
    }

    void Renderer::drawTop(Npi::RObject& t_object, std::function<bool()> t_shadingFunction, Npi::RenderContext::Mode t_mode, int t_layer) {
        if (t_mode == Npi::RenderContext::Mode::_2D) {
            if(m_drawStackTop2d.count(t_layer) > 0) {
                m_drawStackTop2d[t_layer].insert(m_drawStackTop2d[t_layer].end(), std::make_pair(&t_object, t_shadingFunction));
            } else {
                std::vector<std::pair<Npi::RObject*, std::function<bool()>>> newStack;
                newStack.push_back(std::make_pair(&t_object, t_shadingFunction));
                m_drawStackTop2d.insert(std::make_pair(t_layer, newStack));
            }
        } else {
            if(m_drawStackTop3d.count(t_layer) > 0) {
                m_drawStackTop3d[t_layer].insert(m_drawStackTop3d[t_layer].end(), std::make_pair(&t_object, t_shadingFunction));
            } else {
                std::vector<std::pair<Npi::RObject*, std::function<bool()>>> newStack;
                newStack.push_back(std::make_pair(&t_object, t_shadingFunction));
                m_drawStackTop3d.insert(std::make_pair(t_layer, newStack));
            }
        }
    }

    void Renderer::drawBottom(Npi::RObject& t_object, Npi::RenderContext::Mode t_mode, int t_layer) {
        if (t_mode == Npi::RenderContext::Mode::_2D) {
            if(m_drawStackBottom2d.count(t_layer) > 0) {
                m_drawStackBottom2d[t_layer].insert(m_drawStackBottom2d[t_layer].end(), std::make_pair(&t_object, [](){return true;}));
            } else {
                std::vector<std::pair<Npi::RObject*, std::function<bool()>>> newStack;
                newStack.push_back(std::make_pair(&t_object, [](){return true;}));
                m_drawStackBottom2d.insert(std::make_pair(t_layer, newStack));
            }
        } else {
            if(m_drawStackBottom3d.count(t_layer) > 0) {
                m_drawStackBottom3d[t_layer].insert(m_drawStackBottom3d[t_layer].end(), std::make_pair(&t_object, [](){return true;}));
            } else {
                std::vector<std::pair<Npi::RObject*, std::function<bool()>>> newStack;
                newStack.push_back(std::make_pair(&t_object, [](){return true;}));
                m_drawStackBottom3d.insert(std::make_pair(t_layer, newStack));
            }
        }
    }

    void Renderer::drawBottom(Npi::RObject& t_object, std::function<bool()> t_shadingFunction, Npi::RenderContext::Mode t_mode, int t_layer) {
        if (t_mode == Npi::RenderContext::Mode::_2D) {
            if(m_drawStackBottom2d.count(t_layer) > 0) {
                m_drawStackBottom2d[t_layer].insert(m_drawStackBottom2d[t_layer].end(), std::make_pair(&t_object, t_shadingFunction));
            } else {
                std::vector<std::pair<Npi::RObject*, std::function<bool()>>> newStack;
                newStack.push_back(std::make_pair(&t_object, t_shadingFunction));
                m_drawStackBottom2d.insert(std::make_pair(t_layer, newStack));
            }
        } else {
            if(m_drawStackBottom3d.count(t_layer) > 0) {
                m_drawStackBottom3d[t_layer].insert(m_drawStackBottom3d[t_layer].end(), std::make_pair(&t_object, t_shadingFunction));
            } else {
                std::vector<std::pair<Npi::RObject*, std::function<bool()>>> newStack;
                newStack.push_back(std::make_pair(&t_object, t_shadingFunction));
                m_drawStackBottom3d.insert(std::make_pair(t_layer, newStack));
            }
        }
    }


    int Renderer::getScreenWidth(Npi::RenderContext::ScreenTarget t_target) {
        return t_target == Npi::RenderContext::ScreenTarget::Top ? 400 : 320;
    }

    int Renderer::getScreenHeight() {
        return 240;
    }

    void Renderer::setCamera(Npi::Camera t_camera, Npi::RenderContext::ScreenTarget t_target) {
        switch (t_target) {
            case Npi::RenderContext::ScreenTarget::Top:
                m_cameraTop = t_camera;
                break;
            default:
                m_cameraBottom = t_camera;
        }
    }

    Npi::Camera& Renderer::getCamera(Npi::RenderContext::ScreenTarget t_target) {
        return (t_target == Npi::RenderContext::ScreenTarget::Top ? m_cameraTop : m_cameraBottom);
    }

    void Renderer::useFog(bool t_useFog, Npi::RenderContext::ScreenTarget t_target) {
        if (t_target == Npi::RenderContext::ScreenTarget::Top) {
            m_useFogTop = t_useFog;
        } else {
            m_useFogBottom = t_useFog;
        }
    }

    bool Renderer::getUseFog(Npi::RenderContext::ScreenTarget t_target) {
        return (t_target == Npi::RenderContext::ScreenTarget::Top ? m_useFogTop : m_useFogBottom);
    }

    void Renderer::setFogDensity(float t_density, Npi::RenderContext::ScreenTarget t_target) {
        if (t_target == Npi::RenderContext::ScreenTarget::Top) {
            m_fogDensityTop = t_density;
        } else {
            m_fogDensityBottom = t_density;
        }
    }

    float Renderer::getFogDensity(Npi::RenderContext::ScreenTarget t_target) {
        return (t_target == Npi::RenderContext::ScreenTarget::Top ? m_fogDensityTop : m_fogDensityBottom);
    }

    // private methods
    void Renderer::prepare() {
        C3D_BindProgram(rn3d->GetShader());

        // initialize and configure attributes
        C3D_SetAttrInfo(rn3d->GetAttrInfo());

        // Configure the first fragment shading substage to blend the fragment primary color
        // with the fragment secondary color.
        // See https://www.opengl.org/sdk/docs/man2/xhtml/glTexEnv.xml for more insight
        C3D_TexEnv* env = C3D_GetTexEnv(0);
        C3D_TexEnvInit(env);
        C3D_TexEnvSrc(env, C3D_Both, GPU_FRAGMENT_PRIMARY_COLOR, GPU_FRAGMENT_SECONDARY_COLOR, GPU_PRIMARY_COLOR);
        C3D_TexEnvFunc(env, C3D_Both, GPU_ADD);

        C3D_TexEnvInit(C3D_GetTexEnv(2));
        C3D_TexEnvInit(C3D_GetTexEnv(3));
        C3D_TexEnvInit(C3D_GetTexEnv(4));
        C3D_TexEnvInit(C3D_GetTexEnv(5));

        C3D_CullFace(m_useCulling ? GPU_CULL_BACK_CCW : GPU_CULL_NONE);
    }

    void Renderer::prepareFog(Npi::RenderContext::ScreenTarget t_target) {
        switch (t_target) {
            case Npi::RenderContext::ScreenTarget::Bottom:
                FogLut_Exp(&m_fogLutBottom, m_fogDensityBottom, 1.5f, 0.01f, 20.0f);
                C3D_FogGasMode(GPU_FOG, GPU_PLAIN_DENSITY, false);
                C3D_FogColor(m_clearColorTop.getRgb8());
                C3D_FogLutBind(&m_fogLutBottom);
                break;
            default:
                FogLut_Exp(&m_fogLutTop, m_fogDensityTop, 1.5f, 0.01f, 20.0f);
                C3D_FogGasMode(GPU_FOG, GPU_PLAIN_DENSITY, false);
                C3D_FogColor(m_clearColorTop.getRgb8());
                C3D_FogLutBind(&m_fogLutTop);
        }
    }

    void Renderer::prepareLights(Npi::RenderContext::ScreenTarget t_target) {
        switch (t_target) {
            case Npi::RenderContext::ScreenTarget::Bottom:
                C3D_LightEnvBind(&m_lightEnvBottom);
                break;
            default:
                C3D_LightEnvBind(&m_lightEnvTop);
        }
    }
    void Renderer::setWide(bool t_wide)
    {
        this->m_wide = t_wide;
        gfxSetWide(t_wide);
    }
    
}
