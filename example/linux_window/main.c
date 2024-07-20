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

typedef struct {
    Vec3f m_scale;
    Mat4 m_rotation;
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
    transformable->m_rotation = mat4Identity();
    transformable->m_transform = mat4Identity();
    initialize_scale(transformable);
}

void compute_transformation_matrix(Transformable *transformable, Vec3f delta_rotation, Vec3f delta_translation) {
    // Apply rotations
    Mat4 rotateX = mat4RotateX(delta_rotation.x);
    Mat4 rotateY = mat4RotateY(delta_rotation.y);
    Mat4 rotateZ = mat4RotateZ(delta_rotation.z);
    Mat4 rotation = mat4MultiplyM(&rotateY, &rotateX);
    rotation = mat4MultiplyM(&rotateZ, &rotation);
    transformable->m_rotation = mat4MultiplyM(&rotation, &transformable->m_rotation);

    // Apply translation
    transformable->m_translation = vec3fsumV(transformable->m_translation, delta_translation);

    // Compute the final transformation matrix
    Mat4 scale = mat4Scale(transformable->m_scale);
    Mat4 translation_matrix = mat4Translate(transformable->m_translation);
    Mat4 rotation_matrix = transformable->m_rotation;
    
    // Ensure the rotation happens before translation
    transformable->m_transform = mat4MultiplyM(&translation_matrix, &rotation_matrix);
    transformable->m_transform = mat4MultiplyM(&transformable->m_transform, &scale);
}

void compute_transformation_matrix_local(Transformable *transformable, Vec3f delta_rotation, Vec3f delta_translation) {
    // Apply rotations to the camera rotation matrix
    Mat4 rotateX = mat4RotateX(delta_rotation.x);
    Mat4 rotateY = mat4RotateY(delta_rotation.y);
    Mat4 rotateZ = mat4RotateZ(delta_rotation.z);

    Mat4 new_rotation;
    new_rotation = mat4MultiplyM(&rotateY, &rotateX);
    new_rotation = mat4MultiplyM(&rotateZ, &new_rotation);

    transformable->m_rotation = mat4MultiplyM(&new_rotation, &transformable->m_rotation); // Combine new rotation with the current camera rotation

    Vec3f direction = mat4MultiplyVec3(&delta_translation, &transformable->m_rotation); // Transform direction by the camera's rotation matrix
    transformable->m_translation = vec3fsumV(transformable->m_translation, direction);

    Mat4 translation = mat4Translate(transformable->m_translation);
    transformable->m_transform = mat4MultiplyM(&transformable->m_rotation, &translation); // Combine rotation and translation for the view matrix
}

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

    Transformable m_camera;
    initialize(&m_camera);
    m_camera.m_translation = (Vec3f){0, 8, 60};

    renderer.camera_projection = mat4Perspective(3, 50.0, (float)size.x / (float)size.y, 0.1);

    XEvent event;
    XSelectInput(dis, win, KeyPressMask | KeyReleaseMask);

    const int target_fps = 60;
    const unsigned long target_frame_duration = 1000000 / target_fps;
    float pi = 3.14159265359;
    float rotation_speed = 1.0f * pi / 180.0f;
    float move_speed = 1.0f;

    bool running = true;
    while (running) {
        unsigned long frame_start = getMicroseconds();
        XNextEvent(dis, &event);
        Vec3f delta_rotation = {0, 0, 0};
        Vec3f delta_translation = {0, 0, 0};

        if (event.type == KeyPress) {
            KeySym key = XLookupKeysym(&event.xkey, 0);
            if (key == XK_Up) delta_rotation.x = -rotation_speed; m_camera.m_modified = true;
            if (key == XK_Down) delta_rotation.x = rotation_speed; m_camera.m_modified = true;
            if (key == XK_Right) delta_rotation.y = -rotation_speed; m_camera.m_modified = true;
            if (key == XK_Left) delta_rotation.y = rotation_speed; m_camera.m_modified = true;
            if (key == XK_q) delta_rotation.z = rotation_speed; m_camera.m_modified = true;
            if (key == XK_e) delta_rotation.z = -rotation_speed; m_camera.m_modified = true;
            if (key == XK_w) delta_translation.z = -move_speed; m_camera.m_modified = true;
            if (key == XK_s) delta_translation.z = move_speed; m_camera.m_modified = true;
            if (key == XK_a) delta_translation.x = -move_speed; m_camera.m_modified = true;
            if (key == XK_d) delta_translation.x = move_speed; m_camera.m_modified = true;
            if (key == XK_Page_Up) delta_translation.y = move_speed; m_camera.m_modified = true;
            if (key == XK_Page_Down) delta_translation.y = -move_speed; m_camera.m_modified = true;
            if (key == XK_Escape) running = false;
        }

        if (m_camera.m_modified) {
            
            compute_transformation_matrix_local(&m_camera, delta_rotation, delta_translation);

            renderer.camera_view = m_camera.m_transform;

            renderer_render(&renderer);

            // extract the new rotation angles from the rotation matrix
            float camera_angle_x = asin(-m_camera.m_transform.elements[9]);
            float camera_angle_y = atan2(m_camera.m_transform.elements[8], m_camera.m_transform.elements[10]);
            float camera_angle_z = atan2(m_camera.m_transform.elements[1], m_camera.m_transform.elements[5]); 

            // debug print the camera position and rotation angles to the console
            printf("Camera position: %.1f %.1f %.1f", m_camera.m_translation.x, m_camera.m_translation.y, m_camera.m_translation.z);
            printf(" rotation: %.1f %.1f %.1f\n", camera_angle_x * (180.0 / pi), camera_angle_y * (180.0 / pi), camera_angle_z * (180.0 / pi));
        }

        unsigned long frame_end = getMicroseconds();
        unsigned long frame_duration = frame_end - frame_start;
        if (frame_duration < target_frame_duration) {
            usleep(target_frame_duration - frame_duration);
        }
    }

    return 0;
}
