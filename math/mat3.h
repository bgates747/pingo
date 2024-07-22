#pragma once

#include "types.h"
#include "vec2.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Represents a 3x3 matrix.
 */
typedef struct Mat3 {
    F_TYPE elements[9];
} Mat3;

/**
 * @brief Returns an identity matrix.
 * 
 * |1|0|x|
 * |0|1|y|
 * |0|0|1|
 * 
 * @return An identity matrix.
 */
extern Mat3 mat3Identity();

/**
 * @brief Builds a clean translation matrix with x and y translation along relative axes.
 * 
 * |1|0|x|
 * |0|1|y|
 * |0|0|1|
 * 
 * @param l Translation vector.
 * @return A translation matrix.
 */
extern Mat3 mat3Translate(Vec2f l);

/**
 * @brief Builds a clean rotation matrix of Θ angle.
 * 
 * | c(Θ) | -s(Θ)  | 0 |
 * | s(Θ) | c(Θ)   | 0 |
 * | 0    | 0      | 1 |
 * 
 * @param theta Angle in radians.
 * @return A rotation matrix.
 */
extern Mat3 mat3Rotate(float theta);

/**
 * @brief Builds a clean scale matrix with x and y scaling factors.
 * 
 * | x | 0 | 0 |
 * | 0 | y | 0 |
 * | 0 | 0 | 1 |
 * 
 * @param s Scale vector.
 * @return A scale matrix.
 */
extern Mat3 mat3Scale(Vec2f s);

/**
 * @brief Multiplies a 2-component vector by a 3x3 matrix.
 * 
 * @param v Pointer to the vector.
 * @param t Pointer to the matrix.
 * @return The resulting vector.
 */
extern Vec2f mat3Multiply(Vec2f *v, Mat3 *t);

/**
 * @brief Multiplies two 3x3 matrices.
 * 
 * @param v Pointer to the first matrix.
 * @param t Pointer to the second matrix.
 * @return The resulting matrix.
 */
extern Mat3 mat3MultiplyM(Mat3 *v, Mat3 *t);

/**
 * @brief Calculates the homogeneous inverse of a matrix.
 * 
 * @param v Pointer to the matrix.
 * @return The inverse matrix.
 */
extern Mat3 mat3Inverse(Mat3 *v);

/**
 * @brief Calculates a complete transformation matrix with translation, rotation, and scale.
 * 
 * Rotation and scale are applied in reference to the provided origin.
 * 
 * @param origin Origin vector for rotation and scale.
 * @param translation Translation vector.
 * @param scale Scale vector.
 * @param rotation Rotation angle in radians.
 * @return The resulting transformation matrix.
 */
extern Mat3 mat3Complete(Vec2f origin, Vec2f translation, Vec2f scale, float rotation);

/**
 * @brief Calculates the determinant of a matrix.
 * 
 * @param m Pointer to the matrix.
 * @return The determinant.
 */
extern F_TYPE mat3Determinant(Mat3 *m);

/**
 * @brief Checks if a matrix has only translation.
 * 
 * @param m Pointer to the matrix.
 * @return 1 if the matrix has only translation, 0 otherwise.
 */
extern int mat3IsOnlyTranslation(Mat3 *m);

/**
 * @brief Checks if a matrix has only translation and doubles the size.
 * 
 * @param m Pointer to the matrix.
 * @return 1 if the matrix has only translation and doubles the size, 0 otherwise.
 */
extern int mat3IsOnlyTranslationDoubled(Mat3 *m);

#ifdef __cplusplus
}
#endif
