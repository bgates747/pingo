#pragma once

#include "pixel.h"
#include "texture.h"
#include <stdbool.h>

#define MAX_RENDERABLES 32

typedef struct Backend Backend;

typedef struct Renderer {
  Renderable *root_renderables[MAX_RENDERABLES];
  int num_renderables;

  Texture framebuffer;
  Pixel clear_color;
  bool clear;

  Mat4 camera_projection;
  Mat4 camera_view;

  Backend *backend;

} Renderer;

extern int renderer_render(Renderer *);

extern int renderer_init(Renderer *, Vec2i size, Backend *backend);

extern int renderer_add_renderable(Renderer *renderer, Renderable *root);

extern int renderer_remove_renderable(Renderer *renderer, Renderable *root);
