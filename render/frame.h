#pragma once

#include "pixel.h"
#include "renderable.h"
#include "../math/vec2.h"

#ifdef __cplusplus
    extern "C" {
#endif

typedef struct  Frame {
   Vector2I size;
   Pixel * frameBuffer;
} Frame;

extern int frameInit( Frame * f, Vector2I size, Pixel *);

extern Renderable frameAsRenderable( Frame * s);

extern void  frameDraw(Frame * f, Vector2I pos, Pixel color);
extern Pixel frameRead(Frame * f, Vector2I pos);

#ifdef __cplusplus
    }
#endif
