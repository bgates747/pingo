#pragma once

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Represents a 2D vector with integer components.
 */
typedef struct Vec2i {
    I_TYPE x;
    I_TYPE y;
} Vec2i;

/**
 * @brief Represents a 2D vector with floating-point components.
 */
typedef struct {
    F_TYPE x;
    F_TYPE y;
} Vec2f;

/**
 * @brief Sums two 2D vectors with integer components.
 * 
 * @param l The left-hand side vector.
 * @param r The right-hand side vector.
 * @return The resulting vector.
 */
extern Vec2i vector2ISum(Vec2i l, Vec2i r);

/**
 * @brief Converts a 2D vector from integer to floating-point components.
 * 
 * @param v The vector to convert.
 * @return The resulting vector with floating-point components.
 */
extern Vec2f vecItoF(Vec2i v);

/**
 * @brief Converts a 2D vector from floating-point to integer components.
 * 
 * @param v The vector to convert.
 * @return The resulting vector with integer components.
 */
extern Vec2i vecFtoI(Vec2f v);

#ifdef __cplusplus
}
#endif
