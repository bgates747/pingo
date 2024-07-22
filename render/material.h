#pragma once

#include "texture.h"

/**
 * @brief Represents a material with an associated texture.
 */
typedef struct Material {
    Texture *texture; ///< Pointer to the texture associated with the material.
} Material;
