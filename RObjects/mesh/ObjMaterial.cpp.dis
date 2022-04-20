#include "material.hpp"

namespace Npi {
    ObjMaterial::ObjMaterial() :
        m_ambient(51, 51, 51),
        m_diffuse(102, 102, 102),
        m_specular0(127, 127, 127) {
            m_material = new C3D_Material;
            updateMaterial();
        }

    ObjMaterial::ObjMaterial(int kar, int kag, int kab, int kdr, int kdg, int kdb, int ksr, int ksg, int ksb)
    {
         this->setAmbient(kar, kag, kab);
         this->setDiffuse(kdr, kdg, kdb);
         this->setSpecular0(ksr, kdg, ksb);

    }

    ObjMaterial::ObjMaterial(C3D_Material t_material) {
        setMaterial(t_material);
        updateMaterial();
    }

    ObjMaterial::ObjMaterial(const Npi::ObjMaterial& t_source) {
        operator=(t_source);
    }

    ObjMaterial::~Material() {
        delete m_material;
    }

    void ObjMaterial::addPolygon(Npi::Polygon t_polygon) {
        Npi::Polygon::Vertex vertex0 = t_polygon.getVertex(0),
                                   vertex1 = t_polygon.getVertex(1),
                                   vertex2 = t_polygon.getVertex(2);

        m_vertices.push_back({
            { vertex0.position[0], vertex0.position[1], vertex0.position[2] },
            { vertex0.texcoord[0], vertex0.texcoord[1] },
            { vertex0.normal[0], vertex0.normal[1], vertex0.normal[2] },
        });

        m_vertices.push_back({
            { vertex1.position[0], vertex1.position[1], vertex1.position[2] },
            { vertex1.texcoord[0], vertex1.texcoord[1] },
            { vertex1.normal[0], vertex1.normal[1], vertex1.normal[2] },
        });

        m_vertices.push_back({
            { vertex2.position[0], vertex2.position[1], vertex2.position[2] },
            { vertex2.texcoord[0], vertex2.texcoord[1] },
            { vertex2.normal[0], vertex2.normal[1], vertex2.normal[2] },
        });
    }

    void ObjMaterial::setMaterial(C3D_Material t_material) {
        m_ambient.setColor(t_material.ambient[2] * 255, t_material.ambient[1] * 255, t_material.ambient[0] * 255);
        m_diffuse.setColor(t_material.diffuse[2] * 255, t_material.diffuse[1] * 255, t_material.diffuse[0] * 255);
        m_specular0.setColor(t_material.specular0[2] * 255, t_material.specular0[1] * 255, t_material.specular0[0] * 255);
        m_specular1.setColor(t_material.specular1[2] * 255, t_material.specular1[1] * 255, t_material.specular1[0] * 255);
        m_emission.setColor(t_material.emission[2] * 255, t_material.emission[1] * 255, t_material.emission[0] * 255);
        updateMaterial();
    }

    C3D_Material* ObjMaterial::getMaterial() const {
        return m_material;
    }

    void ObjMaterial::setAmbient(Npi::Color t_color) {
        m_ambient = t_color;
        updateMaterial();
    }

    void ObjMaterial::setAmbient(int t_r, int t_g, int t_b) {
        m_ambient = Npi::Color(t_r, t_g, t_b);
        updateMaterial();
    }

    Npi::Color ObjMaterial::getAmbient() {
        return m_ambient;
    }

    void ObjMaterial::setDiffuse(Npi::Color t_color) {
        m_diffuse = t_color;
        updateMaterial();
    }

    void ObjMaterial::setDiffuse(int t_r, int t_g, int t_b) {
        m_diffuse = Npi::Color(t_r, t_g, t_b);
        updateMaterial();
    }

    Npi::Color ObjMaterial::getDiffuse() {
        return m_diffuse;
    }

    void ObjMaterial::setSpecular0(Npi::Color t_color) {
        m_specular0 = t_color;
        updateMaterial();
    }

    void ObjMaterial::setSpecular0(int t_r, int t_g, int t_b) {
        m_specular0 = Npi::Color(t_r, t_g, t_b);
        updateMaterial();
    }

    Npi::Color ObjMaterial::getSpecular0() {
        return m_specular0;
    }

    void ObjMaterial::setSpecular1(Npi::Color t_color) {
        m_specular1 = t_color;
        updateMaterial();
    }

    void ObjMaterial::setSpecular1(int t_r, int t_g, int t_b) {
        m_specular1 = Npi::Color(t_r, t_g, t_b);
        updateMaterial();
    }

    Npi::Color ObjMaterial::getSpecular1() {
        return m_specular1;
    }

    void ObjMaterial::setEmission(Npi::Color t_color) {
        m_emission = t_color;
        updateMaterial();
    }

    void ObjMaterial::setEmission(int t_r, int t_g, int t_b) {
        m_emission = Npi::Color(t_r, t_g, t_b);
        updateMaterial();
    }

    Npi::Color ObjMaterial::getEmission() {
        return m_emission;
    }

    Material& ObjMaterial::operator=(const Material& rhs) {
        setMaterial(*rhs.getMaterial());
        return *this;
    }

    // private methods
    void ObjMaterial::updateMaterial() {
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