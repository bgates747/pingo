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

#include <math.h>
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

int main() {
    Pixel * image = loadTexture("assets/viking.rgba", (Vec2i){1024, 1024});
    
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

    renderer.camera_projection = mat4Perspective(3, 50.0, (float) size.x / (float) size.y, 0.1);

    renderer.camera_view = mat4Translate(camera_position);

    XEvent event;

    XSelectInput(dis, win, KeyPressMask | KeyReleaseMask);

    const int target_fps = 60;
    const unsigned long target_frame_duration = 1000000 / target_fps;

    while (1) {
        unsigned long frame_start = getMicroseconds();

        while (XPending(dis)) {
            XNextEvent(dis, &event);
            if (event.type == KeyPress) {
                KeySym key = XLookupKeysym(&event.xkey, 0);
                // printf("Key pressed: %ld\n", key);
                if (key == XK_Up) { // Up arrow
                    theta -= rotation_speed;
                } 
                if (key == XK_Down) { // Down arrow
                    theta += rotation_speed;
                } 
                if (key == XK_Right) { // Right arrow
                    phi -= rotation_speed;
                } 
                if (key == XK_Left) { // Left arrow
                    phi += rotation_speed;
                } 
                if (key == XK_q) { // Q key 
                    gamma -= rotation_speed;
                }
                if (key == XK_e) { // E key
                    gamma += rotation_speed;
                }
                if (key == XK_w) { // W key - move forward
                    Vec3f forward = {0, 0, -move_speed};
                    Mat4 rotateX = mat4RotateX(theta);
                    Mat4 rotateY = mat4RotateY(phi);
                    Mat4 rotateZ = mat4RotateZ(gamma);
                    Mat4 rotation = mat4MultiplyM(&rotateY, &rotateX);
                    forward = mat4MultiplyVec3(&forward, &rotation);
                    camera_position = vec3fsumV(camera_position, forward);
                } 
                if (key == XK_s) { // S key - move backward
                    Vec3f backward = {0, 0, move_speed};
                    Mat4 rotateX = mat4RotateX(theta);
                    Mat4 rotateY = mat4RotateY(phi);
                    Mat4 rotateZ = mat4RotateZ(gamma);
                    Mat4 rotation = mat4MultiplyM(&rotateY, &rotateX);
                    backward = mat4MultiplyVec3(&backward, &rotation);
                    camera_position = vec3fsumV(camera_position, backward);
                } 
                if (key == XK_a) { // A key - move left
                    Vec3f left = {-move_speed, 0, 0};
                    Mat4 rotateX = mat4RotateX(theta);
                    Mat4 rotateY = mat4RotateY(phi);
                    Mat4 rotateZ = mat4RotateZ(gamma);
                    Mat4 rotation = mat4MultiplyM(&rotateY, &rotateX);
                    left = mat4MultiplyVec3(&left, &rotation);
                    camera_position = vec3fsumV(camera_position, left);
                } 
                if (key == XK_d) { // D key - move right
                    Vec3f right = {move_speed, 0, 0};
                    Mat4 rotateX = mat4RotateX(theta);
                    Mat4 rotateY = mat4RotateY(phi);
                    Mat4 rotateZ = mat4RotateZ(gamma);
                    Mat4 rotation = mat4MultiplyM(&rotateY, &rotateX);
                    right = mat4MultiplyVec3(&right, &rotation);
                    camera_position = vec3fsumV(camera_position, right);
                }
            }
        }

        Mat4 rotateX = mat4RotateX(theta);
        Mat4 rotateY = mat4RotateY(phi);
        Mat4 rotation = mat4MultiplyM(&rotateY, &rotateX); // Note the order
        printf("Camera position: %f %f %f", camera_position.x, camera_position.y, camera_position.z);
        printf(" rotation: %.1f %.1f %.1f\n", theta * (180.0 / M_PI), phi * (180.0 / M_PI), gamma * (180.0 / M_PI));

        Mat4 translation = mat4Translate(camera_position);
        renderer.camera_view = mat4MultiplyM(&rotation, &translation); // Apply rotation first, then translation
        
        // root_entity.transform = mat4Identity(); // No need to transform the object itself
        renderer_render(&renderer);
        
        unsigned long frame_end = getMicroseconds();
        unsigned long frame_duration = frame_end - frame_start;

        if (frame_duration < target_frame_duration) {
            usleep(target_frame_duration - frame_duration);
        }
    }
    
    return 0;
}
