#pragma once

#ifndef OBJECT_H
#define OBJECT_H

#include "renderable.h"
#include "mesh.h"
#include "material.h"
#include "../math/mat4.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Represents a 3D object with a mesh, transformation matrix, and material.
 */

typedef struct Object {
    Material *material;           ///< Pointer to the material used by this object.
    Vec4f *transformed_vertices;  ///< Precomputed transformed vertices.
    Vec3f *face_normals;          ///< Precomputed face normals.
    Vec2f *precomputed_tca;       ///< Precomputed texture coordinates for tca.
    Vec2f *precomputed_tcb;       ///< Precomputed texture coordinates for tcb.
    Vec2f *precomputed_tcc;       ///< Precomputed texture coordinates for tcc.
} Object;

/**
 * @brief Precomputes the transformed vertices for the object.
 * 
 * @param f The object to precompute for.
 * @param transform The transformation matrix.
 * @param positions The positions of the vertices.
 * @param vertex_count The number of vertices.
 */
void precompute_transformed_vertices(Object *f, Mat4 transform, Vec3f *positions, int vertex_count); 

/**
 * @brief Precomputes the face normals for the object.
 * 
 * @param f The object to precompute for.
 * @param positions The positions of the vertices.
 * @param indices The indices of the vertices.
 * @param index_count The number of indices.
 */
void precompute_face_normals(Object *f, Vec3f *positions, uint16_t *indices, int index_count);

/**
 * @brief Precomputes the texture coordinates for the object.
 * 
 * @param f The object to precompute for.
 * @param tex_coords The texture coordinates.
 * @param tex_indices The texture indices.
 * @param index_count The number of indices.
 */
void precompute_texture_coordinates(Object *f, Vec2f *tex_coords, uint16_t *tex_indices, int index_count);

/**
 * @brief Converts an object to a renderable object.
 * 
 * @param object Pointer to the object to convert.
 * @return The resulting renderable object.
 */
Renderable objectAsRenderable(Object *object);

#ifdef __cplusplus
}
#endif

#endif // OBJECT_H

