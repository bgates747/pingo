#pragma once

struct Renderer;

#include "render/backend.h"
#include "math/vec2.h"

typedef struct Pixel Pixel;
typedef struct {
    Backend backend;
    Vec2i size;
} WindowBackend;

void windowBackendInit(WindowBackend * thiss, Vec2i size);
