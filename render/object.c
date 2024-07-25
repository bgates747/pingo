#include "object.h"

void precompute_transformed_vertices(Object *f, Mat4 transform, Vec3f *positions, int vertex_count) {
    f->transformed_vertices = malloc(vertex_count * sizeof(Vec4f));
    for (int i = 0; i < vertex_count; i++) {
        Vec4f v = {positions[i].x, positions[i].y, positions[i].z, 1.0};
        f->transformed_vertices[i] = mat4MultiplyVec4(&v, &transform);
    }
}

void precompute_face_normals(Object *f, Vec3f *positions, uint16_t *indices, int index_count) {
    int face_count = index_count / 3;
    f->face_normals = malloc(face_count * sizeof(Vec3f));
    for (int i = 0; i < index_count; i += 3) {
        Vec3f *ver1 = &positions[indices[i + 0]];
        Vec3f *ver2 = &positions[indices[i + 1]];
        Vec3f *ver3 = &positions[indices[i + 2]];

        Vec3f na = vec3fsubV(*ver1, *ver2);
        Vec3f nb = vec3fsubV(*ver1, *ver3);
        f->face_normals[i / 3] = vec3Normalize(vec3Cross(na, nb));
    }
}

void precompute_texture_coordinates(Object *f, Vec2f *tex_coords, uint16_t *tex_indices, int index_count) {
    int face_count = index_count / 3;
    f->precomputed_tca = malloc(face_count * sizeof(Vec2f));
    f->precomputed_tcb = malloc(face_count * sizeof(Vec2f));
    f->precomputed_tcc = malloc(face_count * sizeof(Vec2f));
    for (int i = 0; i < index_count; i += 3) {
        f->precomputed_tca[i / 3] = tex_coords[tex_indices[i + 0]];
        f->precomputed_tcb[i / 3] = tex_coords[tex_indices[i + 1]];
        f->precomputed_tcc[i / 3] = tex_coords[tex_indices[i + 2]];
    }
}

Renderable objectAsRenderable(Object *object) {
    return (Renderable){.renderableType = RENDERABLE_OBJECT, .impl = object};
}
