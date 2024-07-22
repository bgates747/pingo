#pragma once

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Represents a 3D vector with integer components.
 */
typedef struct Vec3i {
    I_TYPE x;
    I_TYPE y;
    I_TYPE z;
} Vec3i;

/**
 * @brief Represents a 3D vector with floating-point components.
 */
typedef struct Vec3f {
    F_TYPE x;
    F_TYPE y;
    F_TYPE z;
} Vec3f;

/**
 * @brief Creates a 3D vector with specified components.
 * 
 * @param x X component.
 * @param y Y component.
 * @param z Z component.
 * @return The resulting 3D vector.
 */
Vec3f vec3f(float x, float y, float z);

/**
 * @brief Multiplies a 3D vector by a scalar.
 * 
 * @param a The vector.
 * @param b The scalar.
 * @return The resulting vector.
 */
Vec3f vec3fmul(Vec3f a, float b);

/**
 * @brief Sums two 3D vectors.
 * 
 * @param a The first vector.
 * @param b The second vector.
 * @return The resulting vector.
 */
Vec3f vec3fsumV(Vec3f a, Vec3f b);

/**
 * @brief Subtracts the second 3D vector from the first.
 * 
 * @param a The first vector.
 * @param b The second vector.
 * @return The resulting vector.
 */
Vec3f vec3fsubV(Vec3f a, Vec3f b);

/**
 * @brief Adds a scalar to each component of a 3D vector.
 * 
 * @param a The vector.
 * @param b The scalar.
 * @return The resulting vector.
 */
Vec3f vec3fsum(Vec3f a, float b);

/**
 * @brief Computes the dot product of two 3D vectors.
 * 
 * @param a The first vector.
 * @param b The second vector.
 * @return The dot product.
 */
float vec3Dot(Vec3f a, Vec3f b);

/**
 * @brief Computes the cross product of two 3D vectors.
 * 
 * @param a The first vector.
 * @param b The second vector.
 * @return The resulting vector.
 */
Vec3f vec3Cross(Vec3f a, Vec3f b);

/**
 * @brief Normalizes a 3D vector.
 * 
 * @param v The vector to normalize.
 * @return The normalized vector.
 */
Vec3f vec3Normalize(Vec3f v);

#ifdef __cplusplus
}
#endif
