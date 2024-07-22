
#pragma once

#include "render/backend.h"
#include "math/vec2.h"

typedef struct Pixel Pixel;

typedef  struct {
    Backend backend;
    Vec2i size;
} LinuxFramebufferBackend;


void linuxFramebufferBackendInit(LinuxFramebufferBackend * this, Vec2i size, const char *);

