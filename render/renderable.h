#pragma once

#include "../math/mat4.h"

/**
 * @brief Forward declaration of the Renderer structure.
 */
typedef struct Renderer Renderer;

/**
 * @brief Enum representing different types of renderable objects.
 */
typedef enum {
    RENDERABLE_SCENE = 0,
    RENDERABLE_SPRITE,
    RENDERABLE_OBJECT,
    RENDERABLE_COUNT,
} RenderableType;

/**
 * @brief Structure representing a renderable object.
 */
typedef struct {
    RenderableType renderableType;
    void *impl;
} Renderable;

/**
 * @brief Array of function pointers for rendering different types of renderable objects.
 */
extern int (*renderingFunctions[RENDERABLE_COUNT])(Mat4 transform, Renderer *, Renderable);
