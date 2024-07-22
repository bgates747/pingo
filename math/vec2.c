#include "vec2.h"

/**
 * @brief Sums two 2D vectors with integer components.
 * 
 * @param l The left-hand side vector.
 * @param r The right-hand side vector.
 * @return The resulting vector.
 */
Vec2i vector2ISum(Vec2i l, Vec2i r) {
    return (Vec2i){l.x + r.x, l.y + r.y};
}

/**
 * @brief Converts a 2D vector from integer to floating-point components.
 * 
 * @param v The vector to convert.
 * @return The resulting vector with floating-point components.
 */
Vec2f vecItoF(Vec2i v) {
    return (Vec2f){(F_TYPE)v.x, (F_TYPE)v.y};
}

/**
 * @brief Converts a 2D vector from floating-point to integer components.
 * 
 * @param v The vector to convert.
 * @return The resulting vector with integer components.
 */
Vec2i vecFtoI(Vec2f v) {
    return (Vec2i){(I_TYPE)v.x, (I_TYPE)v.y};
}
