#pragma once

#include "../../render/backend.h"
#include "../../math/vec2.h"

typedef struct LinuxBackEnd {
    BackEnd backEnd;
    Vec2i size;
} LinuxBackEnd;

void linuxBackEndInit(LinuxBackEnd *linuxBackend, Vec2i size);
