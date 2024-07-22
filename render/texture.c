#include "texture.h"
#include "math.h"

/**
 * @brief Initializes a texture.
 * 
 * @param f The texture to initialize.
 * @param size The size of the texture.
 * @param buf The pixel data for the texture.
 * @return 0 on success, 1 if the size is invalid, 2 if the buffer is null.
 */
int texture_init(Texture *f, Vec2i size, Pixel *buf) {
    if (size.x * size.y == 0)
        return 1; // 0 sized rect

    if (buf == 0)
        return 2; // null ptr buffer

    f->framebuffer = (Pixel *)buf;
    f->size = size;

    return 0;
}

/**
 * @brief Draws a pixel on the texture.
 * 
 * @param f The texture to draw on.
 * @param pos The position to draw the pixel.
 * @param color The color of the pixel.
 */
void texture_draw(Texture *f, Vec2i pos, Pixel color) {
    f->framebuffer[pos.x + pos.y * f->size.x] = color;
}

/**
 * @brief Reads a pixel from the texture.
 * 
 * @param f The texture to read from.
 * @param pos The position of the pixel to read.
 * @return The pixel at the specified position.
 */
Pixel texture_read(Texture *f, Vec2i pos) {
    return f->framebuffer[pos.x + pos.y * f->size.x];
}

/**
 * @brief Reads a pixel from the texture using floating-point coordinates.
 * 
 * @param f The texture to read from.
 * @param pos The floating-point position of the pixel to read.
 * @return The pixel at the specified position.
 */
Pixel texture_readF(Texture *f, Vec2f pos) {
    uint16_t x = (uint16_t)(pos.x * f->size.x) % f->size.x;
    uint16_t y = (uint16_t)(pos.y * f->size.y) % f->size.x;
    uint32_t index = x + y * f->size.x;
    Pixel value = f->framebuffer[index];
    return value;
}
