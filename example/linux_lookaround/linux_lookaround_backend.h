#pragma once

#include "math/vec2.h"
#include "render/backend.h"

typedef struct LinuxLookaroundBackend {
    Backend backend;
    Vec2i size;
} LinuxLookaroundBackend;

void linuxLookaroundBackendInit(LinuxLookaroundBackend *thiss, Vec2i size);
