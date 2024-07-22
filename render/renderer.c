#include <string.h>
#include <stdio.h>
#include "math/mat4.h"
#include "render/state.h"
#include "renderer.h"
#include "pixel.h"
#include "depth.h"
#include "backend.h"

int renderer_init(Renderer *r, Vec2i size, Backend *backend) {
    memset(r->root_renderables, 0, sizeof(Renderable *) * MAX_RENDERABLES);
    r->num_renderables = 0;
    r->clear = 1;
    r->clear_color = PIXELBLACK;
    r->backend = backend;
    r->backend->init(r, r->backend, (Vec4i) { 0, 0, 0, 0 });

    int e = 0;
    e = texture_init(&r->framebuffer, size, backend->getFrameBuffer(r, backend));
    if (e) return e;

    return 0;
}

int renderer_render(Renderer *r) {
    Backend *be = r->backend;

    int pixels = r->framebuffer.size.x * r->framebuffer.size.y;
    memset(be->getZetaBuffer(r, be), 0, pixels * sizeof(PingoDepth));

    be->beforeRender(r, be);

    // get current framebuffer from Backend
    r->framebuffer.frameBuffer = be->getFrameBuffer(r, be);

    // Clear draw buffer before rendering if necessary
    if (r->clear) {
        memset(be->getFrameBuffer(r, be), 0, pixels * sizeof(Pixel));
    }

    // Render each root renderable
    for (int i = 0; i < r->num_renderables; i++) {
        if (r->root_renderables[i]) {
            r->root_renderables[i]->render(r->root_renderables[i], mat4Identity(), r);
        }
    }

    be->afterRender(r, be);

    return 0;
}

int renderer_add_renderable(Renderer *renderer, Renderable *root) {
    if (renderer->num_renderables >= MAX_RENDERABLES) {
        return -1; // Maximum number of renderables reached
    }
    renderer->root_renderables[renderer->num_renderables++] = root;
    return 0;
}

int renderer_remove_renderable(Renderer *renderer, Renderable *root) {
    for (int i = 0; i < renderer->num_renderables; i++) {
        if (renderer->root_renderables[i] == root) {
            // Shift all elements after the removed renderable
            for (int j = i; j < renderer->num_renderables - 1; j++) {
                renderer->root_renderables[j] = renderer->root_renderables[j + 1];
            }
            renderer->root_renderables[--renderer->num_renderables] = NULL;
            return 0;
        }
    }
    return -1; // Renderable not found
}
