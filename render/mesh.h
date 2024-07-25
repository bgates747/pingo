#pragma once

#ifndef MESH_H
#define MESH_H

#include <stdint.h>
#include "../math/vec2.h"
#include "../math/vec3.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Represents a 3D mesh with vertex positions, texture coordinates, normals, and indices.
 */
typedef struct Mesh {
    int indexes_count;            ///< Number of indices in the mesh.
    Vec3f *positions;             ///< Array of vertex positions.
    Vec2f *textCoord;             ///< Array of texture coordinates.
    Vec3f *normals;               ///< Array of vertex normals.
    uint16_t *pos_indices;        ///< Indices for vertex positions.
    uint16_t *tex_indices;        ///< Indices for texture coordinates.
    uint16_t *norm_indices;       ///< Indices for vertex normals.
} Mesh;

/**
 * @brief Calculates the normals for a given mesh.
 * 
 * @param mesh Pointer to the mesh for which normals will be calculated.
 */
void calculateNormals(Mesh *mesh);

#ifdef __cplusplus
}
#endif

#endif // MESH_H
