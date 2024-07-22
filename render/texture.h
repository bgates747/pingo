#ifndef TEXTURE_H
#define TEXTURE_H

#pragma once

#include "pixel.h"
#include "renderable.h"
#include "../math/vec2.h"

typedef struct Texture {
    Vec2i size;
    Pixel *framebuffer;
} Texture;

extern int texture_init(Texture *f, Vec2i size, Pixel *data);
extern Renderable texture_as_renderable(Texture *s);
extern void texture_draw(Texture *f, Vec2i pos, Pixel color);
extern Pixel texture_read(Texture *f, Vec2i pos);
extern Pixel texture_readF(Texture *f, Vec2f pos);

#endif // TEXTURE_H
