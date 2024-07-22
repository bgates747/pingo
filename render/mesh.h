#pragma once

#include <stdint.h>
#include "../math/vec2.h"
#include "../math/vec3.h"

/**
 * @brief Represents a 3D mesh with vertex positions and texture coordinates.
 */
typedef struct Mesh {
    int indexes_count;         ///< Number of indices in the mesh.
    uint16_t *pos_indices;     ///< Indices for vertex positions.
    uint16_t *tex_indices;     ///< Indices for texture coordinates.
    Vec3f *positions;          ///< Array of vertex positions.
    Vec2f *textCoord;          ///< Array of texture coordinates.
} Mesh;
