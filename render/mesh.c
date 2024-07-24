#include "mesh.h"

void calculateNormals(Mesh *mesh) {
    mesh->normals = malloc(mesh->indexes_count * sizeof(Vec3f));
    for (int i = 0; i < mesh->indexes_count; i += 3) {
        Vec3f *ver1 = &mesh->positions[mesh->pos_indices[i + 0]];
        Vec3f *ver2 = &mesh->positions[mesh->pos_indices[i + 1]];
        Vec3f *ver3 = &mesh->positions[mesh->pos_indices[i + 2]];

        Vec3f na = vec3fsubV(*ver1, *ver2);
        Vec3f nb = vec3fsubV(*ver1, *ver3);
        Vec3f normal = vec3Normalize(vec3Cross(na, nb));

        mesh->normals[i + 0] = normal;
        mesh->normals[i + 1] = normal;
        mesh->normals[i + 2] = normal;
    }
}
