#pragma once

#include "render/backend.h"
#include "math/vec2.h"

typedef  struct TerminalBackend {
    Backend backend;
} TerminalBackend;

void terminal_backend_init(TerminalBackend * t, Vec2i size);
