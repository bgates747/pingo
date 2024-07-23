
#pragma once

#include "render/backend.h"
#include "math/vec2.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/glx.h>
#include <X11/extensions/Xrandr.h>

typedef struct Pixel Pixel;

typedef struct LinuxWindowBackend {
    Backend backend;
    Vec2i size;
    Display *display;
    Window window;
    GLXContext glContext;
} LinuxWindowBackend;

/**
 * @brief Initializes the Linux window backend.
 * 
 * @param backend Pointer to the backend structure.
 * @param size Initial size of the window.
 */
void linuxWindowBackendInit(LinuxWindowBackend *backend, Vec2i size);

/**
 * @brief Gets the current screen refresh rate.
 * 
 * @param backend Pointer to the backend structure.
 * @return int Refresh rate in Hz.
 */
int getRefreshRate(LinuxWindowBackend *backend);

/**
 * @brief Sets up a handler to fire an event on vsync.
 * 
 * @param backend Pointer to the backend structure.
 * @return int 0 on success, non-zero on failure.
 */
int setupVSyncHandler(LinuxWindowBackend *backend);
