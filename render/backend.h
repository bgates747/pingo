#pragma once

#include "../math/vec2.h"
#include "../math/vec4.h"
#include "image.h"
#include "pixel.h"

/**
 * @brief Provides a common interface to multiple graphical backends.
 */
typedef struct Renderer Renderer;
typedef struct PingoDepth PingoDepth;

/**
 * @brief Structure representing a graphical backend.
 */
typedef struct Backend {
    /**
     * @brief Called on initialization and re-initialization.
     * 
     * @param renderer Pointer to the renderer.
     * @param backend Pointer to the backend.
     * @param rect The rectangle area.
     */
    void (*init)(Renderer *, struct Backend *, Vec4i rect);

    /**
     * @brief Called before starting rendering.
     * 
     * @param renderer Pointer to the renderer.
     * @param backend Pointer to the backend.
     */
    void (*beforeRender)(Renderer *, struct Backend *);

    /**
     * @brief Called after having finished a render.
     * 
     * @param renderer Pointer to the renderer.
     * @param backend Pointer to the backend.
     */
    void (*afterRender)(Renderer *, struct Backend *);

    /**
     * @brief Should return the address of the framebuffer (height * width * sizeof(Pixel)).
     * 
     * @param renderer Pointer to the renderer.
     * @param backend Pointer to the backend.
     * @return Pointer to the framebuffer.
     */
    Pixel * (*getFramebuffer)(Renderer *, struct Backend *);

    /**
     * @brief Handle backend specific final framebuffer draw (can apply lighting in a different way if needed).
     * 
     * @param texture Pointer to the texture.
     * @param pos The position to draw the pixel.
     * @param color The color of the pixel.
     * @param illumination The illumination factor.
     */
    void (*drawPixel)(Image * texture, Vec2i pos, Pixel color, float illumination);

    /**
     * @brief Should return the address of the zeta buffer (height * width * sizeof(PingoDepth)).
     * 
     * @param renderer Pointer to the renderer.
     * @param backend Pointer to the backend.
     * @return Pointer to the zeta buffer.
     */
    PingoDepth * (*getZetaBuffer)(Renderer *, struct Backend *);

    /**
     * @brief Allows for referencing client-custom data structure.
     */
    void* clientCustomData;
} Backend;
