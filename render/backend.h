#pragma once

#include "../math/vec2.h"
#include "../math/vec4.h"

/**
  * Provides a common interface to multiple graphical backends
  */

typedef struct tag_Renderer Renderer;
typedef struct tag_Pixel Pixel;
typedef struct tag_PingoDepth PingoDepth;

typedef struct tag_Backend {
    //Called on initialization and re-initialization
    void (*init)(Renderer *, struct tag_Backend *, Vec4i rect);

    //Called before starting rendering
    void (*beforeRender)(Renderer *, struct tag_Backend * );

    //Called after having finished a render
    void (*afterRender)(Renderer *, struct tag_Backend * );

    //Should return the address of the buffer (height*width*sizeof(Pixel))
    Pixel * (*getFramebuffer)(Renderer *, struct tag_Backend * );

    //Handle backend specific final framebuffer draw (can apply lighting in a different way if needed)
    void (*drawPixel)(Texture * f, Vec2i pos, Pixel color, float illumination);

    //Should return the address of the buffer (height*width*sizeof(Pixel))
    PingoDepth * (*getZetaBuffer)(Renderer *, struct tag_Backend * );

    //Allows for referencing client-custom data structure
    void* clientCustomData;
} Backend;
