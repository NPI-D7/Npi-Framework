#pragma once
#include <tuple>
#include <vector>
#include "material.hpp"
#include "RObject.hpp"
#include "color.hpp"
#include "texture.hpp"
#include "vertex.hpp"
#include "polygon.hpp"

namespace Npi {
    /**
     * @brief The base class for all meshes
     */
    class Mesh: public Npi::RObject {
    public:
        /**
         * @brief Initializes the mesh
         */
        Mesh();

        /**
         * @brief Destructor
         */
        ~Mesh();

        /**
         * @brief Adds a polygon to the mesh
         * @param t_polygon The polygon to add
         */
        void addPolygon(Npi::Polygon t_polygon);

        /**
         * @brief Removes all vertices
         */
        void clearVertices();

        /**
         * @brief Sets the rotation on the X-axis
         * @param t_radians  Whether to use radians instead of degrees
         * @param t_rotation The absolute rotation
         */
        void setPitch(float t_rotation, bool t_radians = false);

        /**
         * @brief Returns the rotation on the X-axis
         * @param t_radians Whether to use radians instead of degrees
         * @return The absolute rotation
         */
        float getPitch(bool t_radians = false);

        /**
         * @brief Sets the rotation on the Y-axis
         * @param t_radians  Whether to use radians instead of degrees
         * @param t_rotation The absolute rotation
         */
        void setYaw(float t_rotation, bool t_radians = false);

        /**
         * @brief Returns the rotation on the Y-axis
         * @param t_radians Whether to use radians instead of degrees
         * @return The absolute rotation
         */
        float getYaw(bool t_radians = false);

        /**
         * @brief Sets the rotation on the Z-axis
         * @param t_radians  Whether to use radians instead of degrees
         * @param t_rotation The absolute rotation
         */
        void setRoll(float t_rotation, bool t_radians = false);

        /**
         * @brief Returns the rotation on the /-axis
         * @param t_radians Whether to use radians instead of degrees
         * @return The absolute rotation
         */
        float getRoll(bool t_radians = false);

        /**
         * @brief Sets the rotation of the mesh
         * @param t_pitch    The rotation on the X-axis
         * @param t_yaw      The rotation on the Y-axis
         * @param t_roll     The rotation on the Z-axis
         * @param t_radians  Whether to use radians instead of degrees
         */
        void setRotation(float t_pitch, float t_yaw, float t_roll, bool t_radians = false);

        /**
         * @brief Rotates the mesh a given amount on the X-axis
         * @param t_delta   The amount to rotate
         * @param t_radians Whether to use radians instead of degrees
         */
        void rotatePitch(float t_delta, bool t_radians = false);

        /**
         * @brief Rotates the mesh a given amount on the Y-axis
         * @param t_delta   The amount to rotate
         * @param t_radians Whether to use radians instead of degrees
         */
        void rotateYaw(float t_delta, bool t_radians = false);

        /**
         * @brief Rotates the mesh a given amount on the /-axis
         * @param t_delta   The amount to rotate
         * @param t_radians Whether to use radians instead of degrees
         */
        void rotateRoll(float t_delta, bool t_radians = false);

        /**
         * @brief Sets the position on the X-axis
         * @param t_position The absolute position
         */
        void setPositionX(float t_position);

        /**
         * @brief Returns the current position on the X-axis
         * @return The absolute position
         */
        float getPositionX();

        /**
         * @brief Sets the position on the Y-axis
         * @param t_position The absolute position
         */
        void setPositionY(float t_position);

        /**
         * @brief Returns the current position on the Y-axis
         * @return The absolute position
         */
        float getPositionY();

        /**
         * @brief Sets the position on the Z-axis
         * @param t_position The absolute position
         */
        void setPositionZ(float t_position);

        /**
         * @brief Returns the current position on the Z-axis
         * @return The absolute position
         */
        float getPositionZ();

        /**
         * @brief Sets the position of the mesh
         * @param t_positionX The position on the X-axis
         * @param t_positionY The position on the Y-axis
         * @param t_positionZ The position on the Z-axis
         */
        void setPosition(float t_positionX, float t_positionY, float t_positionZ);

        /**
         * @brief Sets the position of the mesh
         * @param t_position The absolute position, represented as a vector
         */
        void setPosition(Npi::Vector3f t_position);

        /**
         * @brief Moves the mesh along the X-axis
         * @param t_delta The amount to move
         */
        void moveX(float t_delta);

        /**
         * @brief Moves the mesh along the Y-axis
         * @param t_delta The amount to move
         */
        void moveY(float t_delta);

        /**
         * @brief Moves the mesh along the Z-axis
         * @param t_delta The amount to move
         */
        void moveZ(float t_delta);

        /**
         * @brief Sets the scale along the mesh's X-axis
         * @param t_scale The scale
         */
        void setScaleX(float t_scale);

        /**
         * @brief Returns the scale along the mesh's X-axis
         * @return The scale
         */
        float getScaleX();

        /**
         * @brief Sets the scale along the mesh's Y-axis
         * @param t_scale The scale
         */
        void setScaleY(float t_scale);

        /**
         * @brief Returns the scale along the mesh's Y-axis
         * @return The scale
         */
        float getScaleY();

        /**
         * @brief Sets the scale along the mesh's Z-axis
         * @param t_scale The scale
         */
        void setScaleZ(float t_scale);

        /**
         * @brief Returns the scale along the mesh's Z-axis
         * @return The scale
         */
        float getScaleZ();

        /**
         * @brief Sets the scale of the mesh
         * @param t_scaleX The scale along it's X-axis
         * @param t_scaleY The scale along it's Y-axis
         * @param t_scaleZ The scale along it's Z-axis
         */
        void setScale(float t_scaleX, float t_scaleY, float t_scaleZ);

        /**
         * @brief Scales the mesh along it's X-axis
         * @param t_delta The amount to scale
         */
        void scaleX(float t_delta);

        /**
         * @brief Scales the mesh along it's Y-axis
         * @param t_delta The amount to scale
         */
        void scaleY(float t_delta);

        /**
         * @brief Scales the mesh along it's Z-axis
         * @param t_delta The amount to scale
         */
        void scaleZ(float t_delta);

        /**
         * @brief Sets the material of the mesh
         * @param t_material The material
         */
        void setMaterial(Npi::Material& t_material);

        /**
         * @brief Returns the material of the mesh
         * @return The material
         */
        Npi::Material& getMaterial();

        /**
         * @brief Binds a texture to the mesh
         * @param t_texture       The texture to bind
         * @param t_resetMaterial Whether or not to reset the material so that it won't interfere with the texture
         */
        void bindTexture(Npi::Texture& t_texture, bool t_resetMaterial = true);

        /**
         * @brief Unbinds the bound texture (disables texture using)
         * @param t_resetMaterial Whether or not to reset the material to the default material
         */
        void unbindTexture(bool t_resetMaterial = true);

        /**
         * @brief Returns a reference to the current texture
         * @return The current texture (or nullptr if no texture was bound)
         */
        Npi::Texture& getTexture();

        /**
         * @brief Returns whether a texture was bound or not
         * @return `true` if a texture was bound, `false` otherwise
         */
        bool isTextureBound();

        inline std::vector<Npi::Polygon::Vertex> GetVertecies() { return this->m_vertices; }
        inline Npi::Polygon::Vertex* GetVbo() { return this->m_vbo; }

    protected:
        /**
         * @brief Updates the vertex buffer
         */
        void updateVBO();

    private:
        /* data */
        float m_rotationX, m_rotationY, m_rotationZ, m_posX, m_posY, m_posZ, m_scaleX, m_scaleY, m_scaleZ;
        bool m_useTexture;

        // polygons
        std::vector<Npi::Polygon::Vertex> m_vertices;
        Npi::Polygon::Vertex* m_vbo;

        // display properties
        Npi::Texture m_texture;
        Npi::Material m_material;
    };
} /* Npi */