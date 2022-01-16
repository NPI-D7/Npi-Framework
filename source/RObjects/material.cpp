#include "material.hpp"

namespace Npi {
    Material::Material() :
        m_ambient(51, 51, 51),
        m_diffuse(102, 102, 102),
        m_specular0(127, 127, 127) {
            m_material = new C3D_Material;
            updateMaterial();
        }

    Material::Material(int kar, int kag, int kab, int kdr, int kdg, int kdb, int ksr, int ksg, int ksb)
    {
         this->setAmbient(kar, kag, kab);
         this->setDiffuse(kdr, kdg, kdb);
         this->setSpecular0(ksr, kdg, ksb);

    }

    Material::Material(C3D_Material t_material) {
        setMaterial(t_material);
        updateMaterial();
    }

    Material::Material(const Npi::Material& t_source) {
        operator=(t_source);
    }

    Material::~Material() {
        delete m_material;
    }

    void Material::setMaterial(C3D_Material t_material) {
        m_ambient.setColor(t_material.ambient[2] * 255, t_material.ambient[1] * 255, t_material.ambient[0] * 255);
        m_diffuse.setColor(t_material.diffuse[2] * 255, t_material.diffuse[1] * 255, t_material.diffuse[0] * 255);
        m_specular0.setColor(t_material.specular0[2] * 255, t_material.specular0[1] * 255, t_material.specular0[0] * 255);
        m_specular1.setColor(t_material.specular1[2] * 255, t_material.specular1[1] * 255, t_material.specular1[0] * 255);
        m_emission.setColor(t_material.emission[2] * 255, t_material.emission[1] * 255, t_material.emission[0] * 255);
        updateMaterial();
    }

    C3D_Material* Material::getMaterial() const {
        return m_material;
    }

    void Material::setAmbient(Npi::Color t_color) {
        m_ambient = t_color;
        updateMaterial();
    }

    void Material::setAmbient(int t_r, int t_g, int t_b) {
        m_ambient = Npi::Color(t_r, t_g, t_b);
        updateMaterial();
    }

    Npi::Color Material::getAmbient() {
        return m_ambient;
    }

    void Material::setDiffuse(Npi::Color t_color) {
        m_diffuse = t_color;
        updateMaterial();
    }

    void Material::setDiffuse(int t_r, int t_g, int t_b) {
        m_diffuse = Npi::Color(t_r, t_g, t_b);
        updateMaterial();
    }

    Npi::Color Material::getDiffuse() {
        return m_diffuse;
    }

    void Material::setSpecular0(Npi::Color t_color) {
        m_specular0 = t_color;
        updateMaterial();
    }

    void Material::setSpecular0(int t_r, int t_g, int t_b) {
        m_specular0 = Npi::Color(t_r, t_g, t_b);
        updateMaterial();
    }

    Npi::Color Material::getSpecular0() {
        return m_specular0;
    }

    void Material::setSpecular1(Npi::Color t_color) {
        m_specular1 = t_color;
        updateMaterial();
    }

    void Material::setSpecular1(int t_r, int t_g, int t_b) {
        m_specular1 = Npi::Color(t_r, t_g, t_b);
        updateMaterial();
    }

    Npi::Color Material::getSpecular1() {
        return m_specular1;
    }

    void Material::setEmission(Npi::Color t_color) {
        m_emission = t_color;
        updateMaterial();
    }

    void Material::setEmission(int t_r, int t_g, int t_b) {
        m_emission = Npi::Color(t_r, t_g, t_b);
        updateMaterial();
    }

    Npi::Color Material::getEmission() {
        return m_emission;
    }

    Material& Material::operator=(const Material& rhs) {
        setMaterial(*rhs.getMaterial());
        return *this;
    }

    // private methods
    void Material::updateMaterial() {
        m_material->ambient[0] = (float) m_ambient.getBlue() / 255;
        m_material->ambient[1] = (float) m_ambient.getGreen() / 255;
        m_material->ambient[2] = (float) m_ambient.getRed() / 255;

        m_material->diffuse[0] = (float) m_diffuse.getBlue() / 255;
        m_material->diffuse[1] = (float) m_diffuse.getGreen() / 255;
        m_material->diffuse[2] = (float) m_diffuse.getRed() / 255;

        m_material->specular0[0] = (float) m_specular0.getBlue() / 255;
        m_material->specular0[1] = (float) m_specular0.getGreen() / 255;
        m_material->specular0[2] = (float) m_specular0.getRed() / 255;

        m_material->specular1[0] = (float) m_specular1.getBlue() / 255;
        m_material->specular1[1] = (float) m_specular1.getGreen() / 255;
        m_material->specular1[2] = (float) m_specular1.getRed() / 255;

        m_material->emission[0] = (float) m_emission.getBlue() / 255;
        m_material->emission[1] = (float) m_emission.getGreen() / 255;
        m_material->emission[2] = (float) m_emission.getRed() / 255;
    }
} /* Npi */