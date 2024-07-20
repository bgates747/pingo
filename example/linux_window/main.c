#include "math/mat4.h"
#include "math/vec3.h"
#include "assets/viking.h"
#include "linux_window_backend.h"

#include "render/entity.h"
#include "render/material.h"
#include "render/mesh.h"
#include "render/object.h"
#include "render/pixel.h"
#include "render/renderer.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <sys/time.h>

#include <string.h>
#include <math.h>

typedef struct {
    Vec3f m_scale;
    Vec3f m_rotation;
    Vec3f m_translation;
    Mat4 m_transform;
    bool m_modified;
} Transformable;

void initialize_scale(Transformable *transformable) {
    transformable->m_scale.x = 1.0f;
    transformable->m_scale.y = 1.0f;
    transformable->m_scale.z = 1.0f;
    transformable->m_modified = true;
}

void initialize(Transformable *transformable) {
    memset(transformable, 0, sizeof(Transformable));
    initialize_scale(transformable);
}

void compute_transformation_matrix(Transformable *transformable, Vec3f delta_rotation, Vec3f delta_translation) {
    transformable->m_rotation.x += delta_rotation.x;
    transformable->m_rotation.y += delta_rotation.y;
    transformable->m_rotation.z += delta_rotation.z;
    transformable->m_translation.x += delta_translation.x;
    transformable->m_translation.y += delta_translation.y;
    transformable->m_translation.z += delta_translation.z;

    transformable->m_transform = mat4Scale(transformable->m_scale);
    if (transformable->m_rotation.x) {
        Mat4 t = mat4RotateX(transformable->m_rotation.x);
        transformable->m_transform = mat4MultiplyM(&transformable->m_transform, &t);
    }
    if (transformable->m_rotation.y) {
        Mat4 t = mat4RotateY(transformable->m_rotation.y);
        transformable->m_transform = mat4MultiplyM(&transformable->m_transform, &t);
    }
    if (transformable->m_rotation.z) {
        Mat4 t = mat4RotateZ(transformable->m_rotation.z);
        transformable->m_transform = mat4MultiplyM(&transformable->m_transform, &t);
    }
    if (transformable->m_translation.x || transformable->m_translation.y || transformable->m_translation.z) {
        Mat4 t = mat4Translate(transformable->m_translation);
        transformable->m_transform = mat4MultiplyM(&transformable->m_transform, &t);
    }
    transformable->m_modified = false;
}

void compute_transformation_matrix_local(Transformable *transformable, Vec3f delta_rotation, Vec3f delta_translation) {
    // Combine the additions into single operations using vec3fsumV
    transformable->m_rotation = vec3fsumV(transformable->m_rotation, delta_rotation);
    transformable->m_translation = vec3fsumV(transformable->m_translation, delta_translation);

    transformable->m_transform = mat4Scale(transformable->m_scale);
    if (transformable->m_rotation.x) {
        Mat4 t = mat4RotateX(transformable->m_rotation.x);
        transformable->m_transform = mat4MultiplyM(&transformable->m_transform, &t);
    }
    if (transformable->m_rotation.y) {
        Mat4 t = mat4RotateY(transformable->m_rotation.y);
        transformable->m_transform = mat4MultiplyM(&transformable->m_transform, &t);
    }
    if (transformable->m_rotation.z) {
        Mat4 t = mat4RotateZ(transformable->m_rotation.z);
        transformable->m_transform = mat4MultiplyM(&transformable->m_transform, &t);
    }
    if (transformable->m_translation.x || transformable->m_translation.y || transformable->m_translation.z) {
        Mat4 t = mat4Translate(transformable->m_translation);
        transformable->m_transform = mat4MultiplyM(&transformable->m_transform, &t);
    }
    transformable->m_modified = false;
}

extern Display *dis;
extern Window win;

Pixel * loadTexture(char * filename, Vec2i size) {
    Pixel * image = malloc(size.x * size.y * 4);
    FILE * file = fopen(filename, "rb");
    if (file == 0) {
        printf("Error: Could not open file %s\n", filename);
        exit(-1);
    }
    for (int i = 1023; i > 0; i--) {
        for (int j = 0; j < 1024; j++) {
            char r, g, b, a;
            fread(&r, 1, 1, file);
            fread(&g, 1, 1, file);
            fread(&b, 1, 1, file);
            fread(&a, 1, 1, file);
            image[i * 1024 + j] = pixelFromRGBA(r, g, b, a);
        }
    }
    fclose(file);
    return image;
}

unsigned long getMicroseconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}

float pi = 3.14159265359;

int main() {
    Pixel *image = loadTexture("assets/viking.rgba", (Vec2i){1024, 1024});

    Texture texture;
    texture_init(&texture, (Vec2i){1024, 1024}, image);

    Material material;
    material_init(&material, &texture);

    Object object;
    object_init(&object, &viking_mesh, &material);

    Entity root_entity;
    entity_init(&root_entity, (Renderable*)&object, mat4Identity());

    Vec2i size = {640, 480};
    LinuxWindowBackend backend;
    linuxWindowBackendInit(&backend, size);

    Renderer renderer;
    renderer_init(&renderer, size, (Backend*)&backend);
    renderer_set_root_renderable(&renderer, (Renderable*)&root_entity);

    renderer.camera_projection = mat4Perspective(3, 50.0, (float)size.x / (float)size.y, 0.1);

    Transformable m_camera;
    initialize(&m_camera);
    m_camera.m_translation = (Vec3f){0, 0, 50};

    XEvent event;

    XSelectInput(dis, win, KeyPressMask | KeyReleaseMask);

    float pi = 3.14159265359;
    float move_speed = 1.0f;
    float rotation_speed = 1.0f * pi / 180.0f;

    const int target_fps = 60;
    const unsigned long target_frame_duration = 1000000 / target_fps;
    bool running = true;

    while (running) {
        unsigned long frame_start = getMicroseconds();
        XNextEvent(dis, &event);
        if (event.type == KeyPress) {
            bool changed = false;
            KeySym key = XLookupKeysym(&event.xkey, 0);
            Vec3f direction = {0, 0, 0};
            Vec3f rotation = {0, 0, 0};
            if (key == XK_Up) {
                rotation = (Vec3f){-rotation_speed, 0, 0};
                m_camera.m_modified = true;
                changed = true;
            }
            if (key == XK_Down) {
                rotation = (Vec3f){rotation_speed, 0, 0};
                m_camera.m_modified = true;
                changed = true;
            }
            if (key == XK_Right) {
                rotation = (Vec3f){0, -rotation_speed, 0};
                m_camera.m_modified = true;
                changed = true;
            }
            if (key == XK_Left) {
                rotation = (Vec3f){0, rotation_speed, 0};
                m_camera.m_modified = true;
                changed = true;
            }
            if (key == XK_q) {
                rotation = (Vec3f){0, 0, rotation_speed};
                m_camera.m_modified = true;
                changed = true;
            }
            if (key == XK_e) {
                rotation = (Vec3f){0, 0, -rotation_speed};
                m_camera.m_modified = true;
                changed = true;
            }
            if (key == XK_w) {
                direction = (Vec3f){0, 0, -move_speed};
                m_camera.m_modified = true;
                changed = true;
            }
            if (key == XK_s) {
                direction = (Vec3f){0, 0, move_speed};
                m_camera.m_modified = true;
                changed = true;
            }
            if (key == XK_a) {
                direction = (Vec3f){-move_speed, 0, 0};
                m_camera.m_modified = true;
                changed = true;
            }
            if (key == XK_d) {
                direction = (Vec3f){move_speed, 0, 0};
                m_camera.m_modified = true;
                changed = true;
            }
            if (key == XK_Page_Up) {
                direction = (Vec3f){0, move_speed, 0};
                m_camera.m_modified = true;
                changed = true;
            }
            if (key == XK_Page_Down) {
                direction = (Vec3f){0, -move_speed, 0};
                m_camera.m_modified = true;
                changed = true;
            }
            if (key == XK_Escape) {
                running = false;
            }

            if (changed) {
                compute_transformation_matrix_local(&m_camera, rotation, direction);
                renderer.camera_view = m_camera.m_transform;
                renderer_render(&renderer);
                float angle_x = asin(-m_camera.m_transform.elements[9]);
                float angle_y = atan2(m_camera.m_transform.elements[8], m_camera.m_transform.elements[10]);
                float angle_z = atan2(m_camera.m_transform.elements[1], m_camera.m_transform.elements[5]);
                // debug print the camera position and rotation angles to the console
                printf("Camera position: %.1f %.1f %.1f", m_camera.m_translation.x, m_camera.m_translation.y, m_camera.m_translation.z);
                printf(" rotation: %.1f %.1f %.1f\n", angle_x * (180.0 / pi), angle_y * (180.0 / pi), angle_z * (180.0 / pi));
            }
        }

        unsigned long frame_end = getMicroseconds();
        unsigned long frame_duration = frame_end - frame_start;

        if (frame_duration < target_frame_duration) {
            usleep(target_frame_duration - frame_duration);
        }
    }

    return 0;
}
