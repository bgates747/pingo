#pragma once

#include "image.h"
#include "renderable.h"
#include "pixel.h"
#include "../math/vec4.h"

typedef struct Scene Scene;
typedef struct Backend Backend;

/**
 * @brief Represents a renderer responsible for rendering scenes.
 */
typedef struct Renderer {
    Vec4i camera;             ///< Camera viewport.
    Scene *scene;             ///< Pointer to the scene to be rendered.

    Image framebuffer;      ///< Framebuffer for rendering.
    Pixel clearColor;         ///< Color used for clearing the framebuffer.
    int clear;                ///< Flag indicating whether to clear the framebuffer.

    Mat4 camera_projection;   ///< Projection matrix for the camera.
    Mat4 camera_view;         ///< View matrix for the camera.

    Backend *backend;         ///< Pointer to the backend used for rendering.

} Renderer;

/**
 * @brief Renders the current scene using the renderer.
 * 
 * @param renderer Pointer to the renderer.
 * @return 0 on success.
 */
extern int rendererRender(Renderer *renderer);

/**
 * @brief Initializes the renderer.
 * 
 * @param renderer Pointer to the renderer.
 * @param size Size of the rendering viewport.
 * @param backend Pointer to the backend used for rendering.
 * @return 0 on success.
 */
extern int rendererInit(Renderer *renderer, Vec2i size, struct Backend *backend);

/**
 * @brief Sets the scene to be rendered by the renderer.
 * 
 * @param r Pointer to the renderer.
 * @param s Pointer to the scene.
 * @return 0 on success.
 */
extern int rendererSetScene(Renderer *r, Scene *s);

/**
 * @brief Sets the camera viewport for the renderer.
 * 
 * @param r Pointer to the renderer.
 * @param camera The camera viewport.
 * @return 0 on success.
 */
extern int rendererSetCamera(Renderer *r, Vec4i camera);
