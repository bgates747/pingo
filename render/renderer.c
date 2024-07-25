#include <string.h>
#include <stdio.h>
#include "renderer.h"
#include "sprite.h"
#include "pixel.h"
#include "depth.h"
#include "backend.h"
#include "scene.h"
#include "rasterizer.h"
#include "object.h"

#if DEBUG
extern void show_pixel(float x, float y, uint8_t a, uint8_t b, uint8_t g, uint8_t r);
#endif

int renderFrame(Renderer *r, Renderable ren) {
    Image *f = ren.impl;
    return rasterizer_draw_pixel_perfect((Vec2i){0, 0}, r, f);
}

int renderSprite(Mat4 transform, Renderer *r, Renderable ren) {
    Sprite *s = ren.impl;
    Mat4 backUp = s->t;

    // Apply parent transform to the local transform
    s->t = mat4MultiplyM(&s->t, &transform);

    // Apply camera translation
    Mat4 newMat = mat4Translate((Vec3f){-r->camera.x, -r->camera.y, 0});
    s->t = mat4MultiplyM(&s->t, &newMat);

    rasterizer_draw_transformed(s->t, r, &s->frame);
    s->t = backUp;
    return 0;
}

void renderRenderable(Mat4 transform, Renderer *r, Renderable ren) {
    renderingFunctions[ren.renderableType](transform, r, ren);
}

int renderScene(Mat4 transform, Renderer *r, Renderable ren) {
    Scene *s = ren.impl;
    if (!s->visible)
        return 0;

    // Apply hierarchy transform
    Mat4 newTransform = mat4MultiplyM(&s->transform, &transform);
    for (int i = 0; i < s->numberOfRenderables; i++) {
        renderRenderable(newTransform, r, s->renderables[i]);
    }
    return 0;
}

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

int edgeFunction(const Vec2f *a, const Vec2f *b, const Vec2f *c) {
    return (c->x - a->x) * (b->y - a->y) - (c->y - a->y) * (b->x - a->x);
}

float isClockWise(float x1, float y1, float x2, float y2, float x3, float y3) {
    return (y2 - y1) * (x3 - x2) - (y3 - y2) * (x2 - x1);
}

int orient2d(Vec2i a, Vec2i b, Vec2i c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

void backendDrawPixel(Renderer *r, Image *f, Vec2i pos, Pixel color, float illumination) {
    // If backend specifies something..
    if (r->backend->drawPixel != 0) {
        // Draw using the backend
        r->backend->drawPixel(f, pos, color, illumination);
    } else {
        // By default call this
        image_draw(f, pos, pixelMul(color, illumination));
    }
}

int renderObject(Renderer *r, Object object) {
    const Vec2i scrSize = r->framebuffer.size;

    for (int i = 0; i < object.material->size.x * object.material->size.y / 3; i++) {
        Vec4f a = object.transformed_vertices[i * 3 + 0];
        Vec4f b = object.transformed_vertices[i * 3 + 1];
        Vec4f c = object.transformed_vertices[i * 3 + 2];

        Vec2f tca = object.precomputed_tca[i];
        Vec2f tcb = object.precomputed_tcb[i];
        Vec2f tcc = object.precomputed_tcc[i];

        // Use precomputed face normals
        Vec3f normal = object.face_normals[i];
        Vec3f light = vec3Normalize((Vec3f){-8, -5, 5});
        float diffuseLight = (1.0 + vec3Dot(normal, light)) * 0.5;
        diffuseLight = MIN(1.0, MAX(diffuseLight, 0));

        // Convert to device coordinates by perspective division
        a.w = 1.0 / a.w;
        b.w = 1.0 / b.w;
        c.w = 1.0 / c.w;
        a.x *= a.w;
        a.y *= a.w;
        a.z *= a.w;
        b.x *= b.w;
        b.y *= b.w;
        b.z *= b.w;
        c.x *= c.w;
        c.y *= c.w;
        c.z *= c.w;

        float clocking = isClockWise(a.x, a.y, b.x, b.y, c.x, c.y);
        if (clocking >= 0)
            continue;

        // Compute Screen coordinates
        float halfX = scrSize.x / 2;
        float halfY = scrSize.y / 2;
        Vec2i a_s = {a.x * halfX + halfX, a.y * halfY + halfY};
        Vec2i b_s = {b.x * halfX + halfX, b.y * halfY + halfY};
        Vec2i c_s = {c.x * halfX + halfX, c.y * halfY + halfY};

        int32_t minX = MIN(MIN(a_s.x, b_s.x), c_s.x);
        int32_t minY = MIN(MIN(a_s.y, b_s.y), c_s.y);
        int32_t maxX = MAX(MAX(a_s.x, b_s.x), c_s.x);
        int32_t maxY = MAX(MAX(a_s.y, b_s.y), c_s.y);

        minX = MIN(MAX(minX, 0), r->framebuffer.size.x);
        minY = MIN(MAX(minY, 0), r->framebuffer.size.y);
        maxX = MIN(MAX(maxX, 0), r->framebuffer.size.x);
        maxY = MIN(MAX(maxY, 0), r->framebuffer.size.y);

        // Barycentric coordinates at minX/minY corner
        Vec2i minTriangle = {minX, minY};

        int32_t area = orient2d(a_s, b_s, c_s);
        if (area == 0)
            continue;
        float areaInverse = 1.0 / area;

        int32_t A01 = (a_s.y - b_s.y); // Barycentric coordinates steps
        int32_t B01 = (b_s.x - a_s.x); // Barycentric coordinates steps
        int32_t A12 = (b_s.y - c_s.y); // Barycentric coordinates steps
        int32_t B12 = (c_s.x - b_s.x); // Barycentric coordinates steps
        int32_t A20 = (c_s.y - a_s.y); // Barycentric coordinates steps
        int32_t B20 = (a_s.x - c_s.x); // Barycentric coordinates steps

        int32_t w0_row = orient2d(b_s, c_s, minTriangle);
        int32_t w1_row = orient2d(c_s, a_s, minTriangle);
        int32_t w2_row = orient2d(a_s, b_s, minTriangle);

        tca.x /= a.z;
        tca.y /= a.z;
        tcb.x /= b.z;
        tcb.y /= b.z;
        tcc.x /= c.z;
        tcc.y /= c.z;

        for (int16_t y = minY; y < maxY; y++, w0_row += B12, w1_row += B20, w2_row += B01) {
            int32_t w0 = w0_row;
            int32_t w1 = w1_row;
            int32_t w2 = w2_row;

            for (int32_t x = minX; x < maxX; x++, w0 += A12, w1 += A20, w2 += A01) {
                if ((w0 | w1 | w2) < 0)
                    continue;

                float depth = -(w0 * a.z + w1 * b.z + w2 * c.z) * areaInverse;
                if (depth < 0.0 || depth > 1.0)
                    continue;

                if (depth_check(r->backend->getZetaBuffer(r, r->backend), x + y * scrSize.x, 1 - depth))
                    continue;

                depth_write(r->backend->getZetaBuffer(r, r->backend), x + y * scrSize.x, 1 - depth);

                // Image lookup
                float textCoordx = -(w0 * tca.x + w1 * tcb.x + w2 * tcc.x) * areaInverse * depth;
                float textCoordy = -(w0 * tca.y + w1 * tcb.y + w2 * tcc.y) * areaInverse * depth;

                Pixel text = image_readF(object.material->data, (Vec2f){textCoordx, textCoordy});
                backendDrawPixel(r, &r->framebuffer, (Vec2i){x, y}, text, diffuseLight);
            }
        }
    }

    return 0;
}

int rendererInit(Renderer *r, Vec2i size, Backend *backend) {
    renderingFunctions[RENDERABLE_SPRITE] = &renderSprite;
    renderingFunctions[RENDERABLE_SCENE] = &renderScene;
    renderingFunctions[RENDERABLE_OBJECT] = &renderObject;

    r->scene = 0;
    r->clear = 1;
    r->clearColor = PIXELBLACK;
    r->backend = backend;

    r->backend->init(r, r->backend, (Vec4i){0, 0, 0, 0});

    int e = 0;
    e = image_init(&(r->framebuffer), size, backend->getFramebuffer(r, backend));
    if (e)
        return e;

    return 0;
}

int rendererRender(Renderer * r) {

    int pixels = r->framebuffer.size.x * r->framebuffer.size.y;
    memset(r->backend->getZetaBuffer(r,r->backend), 0, pixels * sizeof (PingoDepth));

    r->backend->beforeRender(r, r->backend);

    //get current framebuffer from backend
    r->framebuffer.image = r->backend->getFramebuffer(r, r->backend);

    //Clear draw buffer before rendering
    if (r->clear) {
        memset(r->backend->getFramebuffer(r,r->backend), 0, pixels * sizeof (Pixel));
    }

    renderScene(mat4Identity(), r, sceneAsRenderable(r->scene));

    r->backend->afterRender(r, r->backend);

    return 0;
}

int rendererSetScene(Renderer *r, Scene *s) {
    if (s == 0)
        return 1; // nullptr scene

    r->scene = s;
    return 0;
}

int rendererSetCamera(Renderer *r, Vec4i rect) {
    r->camera = rect;
    r->backend->init(r, r->backend, rect);
    r->framebuffer.size = (Vec2i){rect.z, rect.w};
    return 0;
}
