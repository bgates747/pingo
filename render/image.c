#include "image.h"
#include "math.h"

int image_init(Image *f, Vec2i size, Pixel *data) {
    if (size.x * size.y == 0)
        return 1; // 0 sized rect

    if (data == 0)
        return 2; // null ptr data

    f->image = (Pixel *)data;
    f->size = size;

    return 0;
}

void image_draw(Image *f, Vec2i pos, Pixel color) {
    f->image[pos.x + pos.y * f->size.x] = color;
}

Pixel image_read(Image *f, Vec2i pos) {
    return f->image[pos.x + pos.y * f->size.x];
}

Pixel image_readF(Image *f, Vec2f pos) {
    uint16_t x = (uint16_t)(pos.x * f->size.x) % f->size.x;
    uint16_t y = (uint16_t)(pos.y * f->size.y) % f->size.x;
    uint32_t index = x + y * f->size.x;
    Pixel value = f->image[index];
    return value;
}