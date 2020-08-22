
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "renderer.h"
#include "sprite.h"
#include "pixel.h"
#include "backend.h"
#include "scene.h"


int drawRect(Vector2I off, Renderer *r, Frame * src) {
    Frame des = r->frameBuffer;

    for (int y = 0; y < src->size.y; y++ ) {
        if (y > des.size.y)
            break; //Do not draw outside bounds

        for (int x = 0; x < src->size.x; x++ ) {
            if (x > des.size.x)
                break; //Do not draw outside bounds

            Vector2I srcPos = {x,y};
            Vector2I desPos = vector2ISum(off,srcPos);
            Pixel color = frameRead(src, srcPos);
            frameDraw(&des, desPos, color);
        }
    }
    return 0;
}

int drawRectTransform(Mat3 t, Renderer *r, Frame * src) {
    Frame des = r->frameBuffer;

    Mat3 inv = mat3Inverse(&t);
    // Transform 4 points of frame to frame buffer space
    Vec2f a = (Vec2f){0,0};
    Vec2f b = (Vec2f){src->size.x,0};
    Vec2f c = (Vec2f){0,src->size.y};
    Vec2f d = (Vec2f){src->size.x,src->size.y};

    a = mat3Multiply(&a, &t);
    b = mat3Multiply(&b, &t);
    c = mat3Multiply(&c, &t);
    d = mat3Multiply(&d, &t);

    // .. To find the axis aligned boundig box
    int minX = fmin(fmin(a.x,b.x),fmin(c.x,d.x));
    int minY = fmin(fmin(a.y,b.y),fmin(c.y,d.y));
    int maxX = fmax(fmax(a.x,b.x),fmax(c.x,d.x));
    int maxY = fmax(fmax(a.y,b.y),fmax(c.y,d.y));

    //Now we can iterate over the pixels of the AABB which contain the source frame
    for (int y = minY; y < maxY; y++) {
        for (int x = minX; x < maxX; x++) {
            //Transform the coordinate back to sprite space
            Vector2I desPos = {x,y};
            Vec2f desPosF = vecItoF(desPos);
            Vec2f srcPosF = mat3Multiply(&desPosF,&inv);
            Vector2I srcPosI = vecFtoI(srcPosF);
            if (srcPosF.x < 0) continue;
            if (srcPosF.y < 0) continue;
            if (srcPosF.x > src->size.x) continue;
            if (srcPosF.y > src->size.y) continue;
            Pixel color = frameRead(src, srcPosI);
            frameDraw(&des, desPos, color);
        }
    }

    return 0;
}

int renderFrame(Renderer *r, Renderable ren) {
    Frame * f = ren.impl;
    return drawRect((Vector2I){0,0},r,f);
};

int renderSprite(Renderer *r, Renderable ren) {
    Sprite * s = ren.impl;
    if (mat3IsOnlyTranslation(&s->t)) {
        Vector2I off = {s->t.elements[2], s->t.elements[5]};
        return drawRect(off,r, &s->frame);
    }

    return drawRectTransform(s->t,r,&s->frame);
};



void renderRenderable(Renderer *r, Renderable ren) {
    renderingFunctions[ren.renderableType](r,ren);
};

int renderScene(Renderer *r, Renderable ren) {
    Scene * s = ren.impl;

    for (int i = 0; i < s->numberOfRenderables; i++) {
        renderRenderable(r, s->renderables[i]);
    }
    return 0;
};

int rendererInit(Renderer * r, Vector2I size, BackEnd * backEnd) {
    r->scene = 0;
    r->clearColor = PIXELBLACK;
    r->backEnd = backEnd;

    int e = 0;
    e = frameInit(&(r->frameBuffer), size, backEnd->getFrameBuffer(r, backEnd));
    if (e) return e;

    r->backEnd->init(r, r->backEnd);

    return 0;
}

static int (*renderingFunctions[RENDERABLE_COUNT])(Renderer *, Renderable)={&renderFrame, &renderSprite, &renderScene};

int rendererRender(Renderer * r)
{
    r->backEnd->beforeRender(r, r->backEnd);

    //get current framebuffe from backend
    r->frameBuffer.frameBuffer= r->backEnd->getFrameBuffer(r, r->backEnd);

    //Clear draw buffer before rendering
    Frame des = r->frameBuffer;
    if (r->clear)
        memset(des.frameBuffer,0,des.size.x*des.size.y*sizeof (Pixel));

    renderScene(r, sceneAsRenderable(r->scene));

    r->backEnd->afterRender(r, r->backEnd);

    return 0;
}

int rendererSetScene(Renderer *r, Scene *s)
{
    if (s == 0)
        return 1; //nullptr scene

    r->scene = s;
    return 0;
}




