#include "vec3.h"
#include <math.h>

/**
 * @brief Multiplies a 3D vector by a scalar.
 * 
 * @param a The vector.
 * @param b The scalar.
 * @return The resulting vector.
 */
Vec3f vec3fmul(Vec3f a, float b) {
    a.x = a.x * b;
    a.y = a.y * b;
    a.z = a.z * b;
    return a;
}

/**
 * @brief Sums two 3D vectors.
 * 
 * @param a The first vector.
 * @param b The second vector.
 * @return The resulting vector.
 */
Vec3f vec3fsumV(Vec3f a, Vec3f b) {
    a.x = a.x + b.x;
    a.y = a.y + b.y;
    a.z = a.z + b.z;
    return a;
}

/**
 * @brief Subtracts the second 3D vector from the first.
 * 
 * @param a The first vector.
 * @param b The second vector.
 * @return The resulting vector.
 */
Vec3f vec3fsubV(Vec3f a, Vec3f b) {
    a.x = a.x - b.x;
    a.y = a.y - b.y;
    a.z = a.z - b.z;
    return a;
}

/**
 * @brief Adds a scalar to each component of a 3D vector.
 * 
 * @param a The vector.
 * @param b The scalar.
 * @return The resulting vector.
 */
Vec3f vec3fsum(Vec3f a, float b) {
    a.x = a.x + b;
    a.y = a.y + b;
    a.z = a.z + b;
    return a;
}

/**
 * @brief Computes the dot product of two 3D vectors.
 * 
 * @param a The first vector.
 * @param b The second vector.
 * @return The dot product.
 */
float vec3Dot(Vec3f a, Vec3f b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

/**
 * @brief Creates a 3D vector with specified components.
 * 
 * @param x X component.
 * @param y Y component.
 * @param z Z component.
 * @return The resulting 3D vector.
 */
Vec3f vec3f(float x, float y, float z) {
    return (Vec3f){x, y, z};
}

/**
 * @brief Computes the cross product of two 3D vectors.
 * 
 * @param a The first vector.
 * @param b The second vector.
 * @return The resulting vector.
 */
Vec3f vec3Cross(Vec3f a, Vec3f b) {
    return (Vec3f) {a.y * b.z - b.y * a.z,
                    a.z * b.x - b.z * a.x,
                    a.x * b.y - b.x * a.y};
}

/**
 * @brief Normalizes a 3D vector.
 * 
 * @param v The vector to normalize.
 * @return The normalized vector.
 */
Vec3f vec3Normalize(Vec3f v) {
    float sqrt = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    return (Vec3f){v.x / sqrt, v.y / sqrt, v.z / sqrt};
}
