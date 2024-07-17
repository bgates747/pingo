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

    float phi = 0;
    float theta = 0;
    float gamma = 0;
    Vec3f camera_position = {0, 8, 60}; // Initial camera position
    float move_speed = 3.0f;
    float rotation_speed = 0.05f;

    renderer.camera_projection = mat4Perspective(3, 50.0, (float)size.x / (float)size.y, 0.1);

    Mat4 camera_rotation = mat4Identity(); // Initialize the camera rotation matrix

    XEvent event;

    XSelectInput(dis, win, KeyPressMask | KeyReleaseMask);

    const int target_fps = 60;
    const unsigned long target_frame_duration = 1000000 / target_fps;

    while (1) {
        unsigned long frame_start = getMicroseconds();
        XNextEvent(dis, &event);
        if (event.type == KeyPress) {
            bool changed = false;
            KeySym key = XLookupKeysym(&event.xkey, 0);
            if (key == XK_Up) {
                theta = -rotation_speed;
                changed = true;
            }
            if (key == XK_Down) {
                theta = rotation_speed;
                changed = true;
            }
            if (key == XK_Right) {
                phi = -rotation_speed;
                changed = true;
            }
            if (key == XK_Left) {
                phi = rotation_speed;
                changed = true;
            }
            if (key == XK_q) {
                gamma = rotation_speed;
                changed = true;
            }
            if (key == XK_e) {
                gamma = -rotation_speed;
                changed = true;
            }
            Vec3f direction = {0, 0, 0};
            if (key == XK_w) {
                direction = (Vec3f){0, 0, -move_speed};
                changed = true;
            }
            if (key == XK_s) {
                direction = (Vec3f){0, 0, move_speed};
                changed = true;
            }
            if (key == XK_a) {
                direction = (Vec3f){-move_speed, 0, 0};
                changed = true;
            }
            if (key == XK_d) {
                direction = (Vec3f){move_speed, 0, 0};
                changed = true;
            }
            if (key == XK_Page_Up) {
                direction = (Vec3f){0, move_speed, 0};
                changed = true;
            }
            if (key == XK_Page_Down) {
                direction = (Vec3f){0, -move_speed, 0};
                changed = true;
            }

            if (changed) {
                // Apply rotations to the camera rotation matrix
                Mat4 rotateX = mat4RotateX(theta);
                Mat4 rotateY = mat4RotateY(phi);
                Mat4 rotateZ = mat4RotateZ(gamma);

                Mat4 new_rotation;
                new_rotation = mat4MultiplyM(&rotateY, &rotateX);
                new_rotation = mat4MultiplyM(&rotateZ, &new_rotation);

                camera_rotation = mat4MultiplyM(&new_rotation, &camera_rotation); // Combine new rotation with the current camera rotation

                direction = mat4MultiplyVec3(&direction, &camera_rotation); // Transform direction by the camera's rotation matrix
                camera_position = vec3fsumV(camera_position, direction);

                Mat4 translation = mat4Translate(camera_position);
                renderer.camera_view = mat4MultiplyM(&camera_rotation, &translation); // Combine rotation and translation for the view matrix

                printf("Camera position: %f %f %f", camera_position.x, camera_position.y, camera_position.z);
                printf(" rotation: %.1f %.1f %.1f\n", theta * (180.0 / pi), phi * (180.0 / pi), gamma * (180.0 / pi));

                renderer_render(&renderer);

                // Reset rotation angles for next frame
                theta = 0;
                phi = 0;
                gamma = 0;
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
