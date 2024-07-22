#ifndef TEXTURE_H
#define TEXTURE_H

#pragma once

#include "pixel.h"
#include "renderable.h"
#include "../math/vec2.h"

/**
 * @brief Represents a texture with a size and framebuffer.
 */
typedef struct Texture {
    Vec2i size;
    Pixel *framebuffer;
} Texture;

/**
 * @brief Initializes a texture.
 * 
 * @param f The texture to initialize.
 * @param size The size of the texture.
 * @param data The pixel data for the texture.
 * @return 0 on success, 1 if the size is invalid, 2 if the buffer is null.
 */
extern int texture_init(Texture *f, Vec2i size, Pixel *data);

/**
 * @brief Converts a texture to a renderable object.
 * 
 * @param s The texture to convert.
 * @return The resulting renderable object.
 */
extern Renderable texture_as_renderable(Texture *s);

/**
 * @brief Draws a pixel on the texture.
 * 
 * @param f The texture to draw on.
 * @param pos The position to draw the pixel.
 * @param color The color of the pixel.
 */
extern void texture_draw(Texture *f, Vec2i pos, Pixel color);

/**
 * @brief Reads a pixel from the texture.
 * 
 * @param f The texture to read from.
 * @param pos The position of the pixel to read.
 * @return The pixel at the specified position.
 */
extern Pixel texture_read(Texture *f, Vec2i pos);

/**
 * @brief Reads a pixel from the texture using floating-point coordinates.
 * 
 * @param f The texture to read from.
 * @param pos The floating-point position of the pixel to read.
 * @return The pixel at the specified position.
 */
extern Pixel texture_readF(Texture *f, Vec2f pos);

#endif // TEXTURE_H
