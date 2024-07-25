#pragma once

#include "image.h"
#include "renderable.h"
#include "../math/mat4.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Represents a sprite with a transformation matrix and a texture frame.
 */
typedef struct Sprite {
    Mat4 t;       ///< Transformation matrix for the sprite.
    Image frame; ///< Image frame of the sprite.
} Sprite;

/**
 * @brief Initializes a sprite.
 * 
 * @param s Pointer to the sprite to initialize.
 * @param f Image frame for the sprite.
 * @param t Transformation matrix for the sprite.
 * @return 0 on success, 1 if the framebuffer is null.
 */
extern int spriteInit(Sprite *s, Image f, Mat4 t);

/**
 * @brief Randomizes the pixels of the sprite's texture frame.
 * 
 * @param s Pointer to the sprite to randomize.
 * @return 0 on success.
 */
extern int spriteRandomize(Sprite *s);

/**
 * @brief Converts a sprite to a renderable object.
 * 
 * @param s Pointer to the sprite to convert.
 * @return The resulting renderable object.
 */
extern Renderable spriteAsRenderable(Sprite *s);

#ifdef __cplusplus
}
#endif
