#pragma once
#include <citro3d.h>
#include "color.hpp"
#include "polygon.hpp"

namespace Npi {
    
    class ObjMaterial {
    public:
        
        ObjMaterial();
        ObjMaterial(int kar, int kag, int kab, int kdr, int kdg, int kdb, int ksr, int ksg, int ksb);
        ObjMaterial(C3D_Material t_material);
        ObjMaterial(const Npi::Material& t_source);
        /**
         * @brief Adds a polygon to the mesh
         * @param t_polygon The polygon to add
         */
        void addPolygon(Npi::Polygon t_polygon);
        virtual ~ObjMaterial();
        void setMaterial(C3D_Material t_material);
        C3D_Material* getMaterial() const;
        void setAmbient(Npi::Color t_color);
        void setAmbient(int t_r, int t_g, int t_b);
        Npi::Color getAmbient();
        void setDiffuse(Npi::Color t_color);
        void setDiffuse(int t_r, int t_g, int t_b);
        Npi::Color getDiffuse();
        void setSpecular0(Npi::Color t_color);
        void setSpecular0(int t_r, int t_g, int t_b);
        Npi::Color getSpecular0();
        void setSpecular1(Npi::Color t_color);
        void setSpecular1(int t_r, int t_g, int t_b);
        Npi::Color getSpecular1();
        void setEmission(Npi::Color t_color);
        void setEmission(int t_r, int t_g, int t_b);
        Npi::Color getEmission();
        Npi::Material& operator=(const Npi::Material& rhs);

        inline std::vector<Npi::Polygon::Vertex> GetVertecies() { return this->m_vertices; }
        inline Npi::Polygon::Vertex* GetVbo() { return this->m_vbo; }

    private:
        void updateMaterial();

        /* data */
        Npi::Texture mttex;
        std::vector<Npi::Polygon::Vertex> m_vertices;
        Npi::Polygon::Vertex* m_vbo;

        Npi::Color m_ambient, m_diffuse, m_specular0, m_specular1, m_emission;
        C3D_Material* m_material;
    };
} /* Npi */
