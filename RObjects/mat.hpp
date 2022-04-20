#pragma once
#include "VertexArray.hpp"
#include "texture.hpp"
#include "color.hpp"
#include <tuple>

class Mat
{
public:
    enum class FragmentColor {
        AMBIENT = 0,
        DIFFUSE = 1,
        SPECULAR0 = 2,
        SPECULAR1 = 3,
        EMISSION = 4
    };
    Mat();
    Mat(std::string name);
    ~Mat();
    std::string& GetName();
    Mat& AddFace(const Npi::Polygon& face);
    Mat& SetTexture(const std::string& fileName, bool toVram = true);
    Mat& SetCostantColor(u32 id, const Npi::Color& color);
    Mat& SetFragmentColor(FragmentColor id, const Npi::Color& color);
    const Npi::Color& GetConstantColor(u32 id);
    const Npi::Color& GetFragmentColor(FragmentColor id);
    Npi::VertexArray& GetVertexArray();
    Mat& SetVisible(bool visible);
    void RunMaterialCallbacks();
    void draw(Npi::RenderContext t_context);

    inline Mat& SetTextureWrapMode(GPU_TEXTURE_WRAP_PARAM sWrap, GPU_TEXTURE_WRAP_PARAM tWrap) {texture->Wrap(sWrap, tWrap); return *this;}

    using BlendMode = void(*)(Mat&, void*);
    using TexCombineMode = void(*)(Mat&, void*);
    using FragmentMode = void(*)(Mat&, void*);

    static void BlendModeOpacity(Mat& mat, void* data);
    static void BlendModeTransparency(Mat& mat, void* data);
    static void BlendModeBlendedTransparency(Mat& mat, void* data);
    static void BlendModeAdditiveTransparency(Mat& mat, void* data);

    Mat& SetBlendMode(BlendMode mode, void* data);

    static void TexCombineModeDeafult(Mat& mat, void* data);
    static void TexCombineModeNoTex(Mat& mat, void* data);
    static void TexCombineModeFont(Mat& mat, void* data);

    Mat& SetTexCombineMode(TexCombineMode mode, void* data);

    static void FragmentModeNone(Mat& mat, void* data);
    static void FragmentModePhong(Mat& mat, void* data);

    Mat& SetFragmentMode(FragmentMode mode, void* data);
    static void InitFragmentLighting();

    void ConvertToVBO(void);
friend class Obj;
private:
    Npi::Texture* texture;
    std::string name;
    std::vector<Npi::Polygon> faces;
    bool isVisible;
    Npi::VertexArray* vArray;
    BlendMode bMode;
    void* blendUsrData;
    TexCombineMode tMode;
    void* texCombineUsrData;
    FragmentMode fMode;
    void* fragModeUsrData;
    Npi::Color constantColors[5];
    Npi::Color fragmentColors[5];
    C3D_LightEnv lightEnv;
    C3D_Light light;
};