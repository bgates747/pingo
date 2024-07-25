#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H

#include "image.h"  
#include "../math/vec2.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Material {
    Vec2i size;                   ///< Dimensions of the texture image bitmap.
    Image *data;                  ///< Pointer to texture image data.
} Material;

#ifdef __cplusplus
}
#endif

#endif // MATERIAL_H
