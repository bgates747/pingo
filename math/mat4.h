#pragma once

#include "types.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Represents a 4x4 matrix.
 */
typedef struct Mat4 {
    F_TYPE elements[16];
} Mat4;

/**
 * @brief Returns an identity matrix.
 * 
 * |1|0|0|0|
 * |0|1|0|0|
 * |0|0|1|0|
 * |0|0|0|1|
 * 
 * @return An identity matrix.
 */
extern Mat4 mat4Identity();

/**
 * @brief Builds a translation matrix with x, y, and z translation along relative axes.
 * 
 * |1|0|0|x|
 * |0|1|0|y|
 * |0|0|1|z|
 * |0|0|0|1|
 * 
 * @param l Translation vector.
 * @return A translation matrix.
 */
extern Mat4 mat4Translate(Vec3f l);

/**
 * @brief Builds a rotation matrix for rotation around the X axis.
 * 
 * |1|0    |0     |0|
 * |0|c(φ) |-s(φ) |0|
 * |0|s(φ) |c(φ)  |0|
 * |0|0    |0     |1|
 * 
 * @param phi Angle in radians.
 * @return A rotation matrix.
 */
extern Mat4 mat4RotateX(F_TYPE phi);

/**
 * @brief Builds a rotation matrix for rotation around the Y axis.
 * 
 * | c(φ) |0|s(φ)|0|
 * | 0    |1|0   |0|
 * |-s(φ) |0|c(φ)|0|
 * | 0    |0|0   |1|
 * 
 * @param phi Angle in radians.
 * @return A rotation matrix.
 */
extern Mat4 mat4RotateY(F_TYPE phi);

/**
 * @brief Builds a rotation matrix for rotation around the Z axis.
 * 
 * |c(φ)  |-s(φ)|0|0|
 * |s(φ)  |c(φ) |0|0|
 * |0     |0    |1|0|
 * |0     |0    |0|1|
 * 
 * @param phi Angle in radians.
 * @return A rotation matrix.
 */
extern Mat4 mat4RotateZ(F_TYPE phi);

/**
 * @brief Multiplies a 2-component vector by a 4x4 matrix.
 * 
 * @param v Pointer to the vector.
 * @param t Pointer to the matrix.
 * @return The resulting vector.
 */
extern Vec2f mat4MultiplyVec2(Vec2f *v, Mat4 *t);

/**
 * @brief Multiplies a 3-component vector by a 4x4 matrix.
 * 
 * @param v Pointer to the vector.
 * @param t Pointer to the matrix.
 * @return The resulting vector.
 */
extern Vec3f mat4MultiplyVec3(Vec3f *v, Mat4 *t);

/**
 * @brief Multiplies a 4-component vector by a 4x4 matrix.
 * 
 * @param v Pointer to the vector.
 * @param t Pointer to the matrix.
 * @return The resulting vector.
 */
extern Vec4f mat4MultiplyVec4(Vec4f *v, Mat4 *t);

/**
 * @brief Multiplies a 4-component vector by a 4x4 matrix (in-place).
 * 
 * @param v Pointer to the vector.
 * @param t Pointer to the matrix.
 * @return The resulting vector.
 */
extern Vec4f mat4MultiplyVec4in(Vec4f *v, Mat4 *t);

/**
 * @brief Multiplies two 4x4 matrices.
 * 
 * @param m1 Pointer to the first matrix.
 * @param m2 Pointer to the second matrix.
 * @return The resulting matrix.
 */
extern Mat4 mat4MultiplyM(Mat4 *m1, Mat4 *m2);

/**
 * @brief Calculates the inverse of a matrix.
 * 
 * @param mat Pointer to the matrix.
 * @return The inverse matrix.
 */
extern Mat4 mat4Inverse(Mat4 *mat);

/**
 * @brief Builds a scale matrix with x, y, and z scaling factors.
 * 
 * | x | 0 | 0 | 0 |
 * | 0 | y | 0 | 0 |
 * | 0 | 0 | z | 0 |
 * | 0 | 0 | 0 | 1 |
 * 
 * @param s Scale vector.
 * @return A scale matrix.
 */
extern Mat4 mat4Scale(Vec3f s);

/**
 * @brief Builds a perspective projection matrix.
 * 
 * @param near The near clipping plane.
 * @param far The far clipping plane.
 * @param aspect The aspect ratio.
 * @param fov The field of view in radians.
 * @return A perspective projection matrix.
 */
extern Mat4 mat4Perspective(float near, float far, float aspect, float fov);

/**
 * @brief Extracts the near clipping plane distance from a projection matrix.
 * 
 * @param mat The projection matrix.
 * @return The near clipping plane distance.
 */
extern float mat4NearFromProjection(Mat4 mat);

/**
 * @brief Extracts the far clipping plane distance from a projection matrix.
 * 
 * @param mat The projection matrix.
 * @return The far clipping plane distance.
 */
extern float mat4FarFromProjection(Mat4 mat);

#ifdef __cplusplus
}
#endif
