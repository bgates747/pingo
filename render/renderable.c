#include "renderable.h"
#include "renderer.h"

/**
 * @brief Array of function pointers for rendering different types of renderable objects.
 */
int (*renderingFunctions[RENDERABLE_COUNT])(Mat4 transform, Renderer *, Renderable);
