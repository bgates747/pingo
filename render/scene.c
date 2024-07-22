#include "scene.h"
#include "renderer.h"

/**
 * @brief Adds a renderable object to the scene.
 * 
 * @param scene Pointer to the scene.
 * @param renderable The renderable object to add.
 * @return 0 on success, 1 if the scene already contains the maximum number of renderables.
 */
int sceneAddRenderable(Scene *scene, Renderable renderable) {
    if (scene->numberOfRenderables >= MAX_SCENE_RENDERABLES) {
        return 1; // Too many renderables in this scene
    }

    scene->renderables[scene->numberOfRenderables++] = renderable;
    return 0;
}

/**
 * @brief Initializes a scene.
 * 
 * @param s Pointer to the scene to initialize.
 * @return 0 on success.
 */
int sceneInit(Scene *s) {
    s->transform = mat4Identity();
    s->numberOfRenderables = 0;
    s->visible = 1;

    return 0;
}

/**
 * @brief Converts a scene to a renderable object.
 * 
 * @param scene Pointer to the scene to convert.
 * @return The resulting renderable object.
 */
extern Renderable sceneAsRenderable(Scene *scene) {
    return (Renderable){.renderableType = RENDERABLE_SCENE, .impl = scene};
}
