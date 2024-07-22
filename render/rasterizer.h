#pragma once

#include "texture.h"
#include "sprite.h"
#include "renderable.h"

/**
  * @brief Defines the type of filtering used when textures are resized and rotated
  * by a transformation. Nearest filtering just takes 1 texture sample in the
  * single pixel which the source coordinate is transformed to.
  * Bilinear filtering takes 4 samples on the source texture to compute a
  * weighted average of those 4 values based on the distance of the input
  * point.
  * Anisotropic filtering takes 4/16 samples from the source image
  * distributed uniformly over the area occupied by the destination pixel on
  * the source image and makes an average of those values.
  */

#define FILTERING_NEAREST
//#define FILTERING_BILINEAR
//#define FILTERING_ANISOTROPIC
//#define FILTERING_ANISOTROPICX2

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Draws a texture pixel-perfectly to the renderer's framebuffer at the specified offset.
 *
 * @param off The offset position to draw the texture.
 * @param r The renderer to draw on.
 * @param src The source texture to draw.
 * @return int Returns 0 on success.
 */
int rasterizer_draw_pixel_perfect(Vec2i off, Renderer *r, Texture *src);

/**
 * @brief Draws a texture pixel-perfectly to the renderer's framebuffer at the specified offset, doubling the size of each pixel.
 *
 * @param off The offset position to draw the texture.
 * @param r The renderer to draw on.
 * @param src The source texture to draw.
 * @return int Returns 0 on success.
 */
int rasterizer_draw_pixel_perfect_doubled(Vec2i off, Renderer *r, Texture *src);

/**
 * @brief Applies a transformation matrix to the texture and draws the transformed texture onto the renderer's framebuffer.
 *
 * @param t The transformation matrix to apply.
 * @param r The renderer to draw on.
 * @param src The source texture to draw.
 * @return int Returns 0 on success.
 */
int rasterizer_draw_transformed(Mat4 t, Renderer *r, Texture *src);

#ifdef __cplusplus
}
#endif
