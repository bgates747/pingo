#pragma once

#include <stdint.h>
#include "renderable.h"
#include "../math/mat3.h"
#include "../math/mat4.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_SCENE_RENDERABLES 32

/**
 * @brief Represents a scene containing multiple renderable objects.
 */
typedef struct Scene {
    uint8_t numberOfRenderables; ///< Number of renderables in the scene.
    Renderable renderables[MAX_SCENE_RENDERABLES]; ///< Array of renderables in the scene.
    Mat4 transform; ///< Transformation matrix for the scene.
    uint8_t visible; ///< Visibility flag for the scene.
} Scene;

/**
 * @brief Initializes a scene.
 * 
 * @param s Pointer to the scene to initialize.
 * @return 0 on success.
 */
extern int sceneInit(Scene *s);

/**
 * @brief Adds a renderable object to the scene.
 * 
 * @param scene Pointer to the scene.
 * @param renderable The renderable object to add.
 * @return 0 on success, 1 if the scene already contains the maximum number of renderables.
 */
extern int sceneAddRenderable(Scene *scene, Renderable renderable);

/**
 * @brief Converts a scene to a renderable object.
 * 
 * @param scene Pointer to the scene to convert.
 * @return The resulting renderable object.
 */
extern Renderable sceneAsRenderable(Scene *scene);

#ifdef __cplusplus
}
#endif
