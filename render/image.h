#ifndef IMAGE_H
#define IMAGE_H

#pragma once

#include "pixel.h"
#include "../math/vec2.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Image data structure.
 */
typedef struct Image {
    Vec2i size;             ///< Dimensions of the image.
    Pixel *image;           ///< Pointer to the image data.
} Image;

/**
 * @brief Initializes a image.
 * 
 * @param f The image to initialize.
 * @param size The size of the image.
 * @param image The image data for the image.
 * @return 0 on success, 1 if the size is invalid, 2 if the buffer is null.
 */
extern int image_init(Image *f, Vec2i size, Pixel *image);

/**
 * @brief Draws a pixel on the image.
 * 
 * @param f The image to draw on.
 * @param pos The position to draw the pixel.
 * @param color The color of the pixel.
 */
extern void image_draw(Image *f, Vec2i pos, Pixel color);

/**
 * @brief Reads a pixel from the image.
 * 
 * @param f The image to read from.
 * @param pos The position of the pixel to read.
 * @return The pixel at the specified position.
 */
extern Pixel image_read(Image *f, Vec2i pos);

/**
 * @brief Reads a pixel from the image using floating-point coordinates.
 * 
 * @param f The image to read from.
 * @param pos The floating-point position of the pixel to read.
 * @return The pixel at the specified position.
 */
extern Pixel image_readF(Image *f, Vec2f pos);

#ifdef __cplusplus
}
#endif

#endif // IMAGE_H
