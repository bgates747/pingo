#pragma once

#include "../math/mat4.h"
#include "mesh.h"
#include "renderable.h"
#include "material.h"

/**
 * @brief Represents a 3D object with a mesh, transformation matrix, material, and texture coordinates.
 */
typedef struct Object {
    Mesh *mesh;              ///< Pointer to the mesh of the object.
    Mat4 transform;          ///< Transformation matrix for the object.
    Material *material;      ///< Pointer to the material of the object.
    Vec2f *textCoord;        ///< Array of texture coordinates.
} Object;

/**
 * @brief Converts an object to a renderable object.
 * 
 * @param object Pointer to the object to convert.
 * @return The resulting renderable object.
 */
Renderable object_as_renderable(Object *object);
