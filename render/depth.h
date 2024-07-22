#pragma once

#include <stdint.h>
#include <stdbool.h>

#define ZBUFFER32 // [ZBUFFER32 | ZBUFFER16 | ZBUFFER8]

#ifdef ZBUFFER32
typedef struct tag_PingoDepth {
    uint32_t d;
} PingoDepth;
#endif

#ifdef ZBUFFER16
typedef struct tag_PingoDepth {
    uint16_t d;
} PingoDepth;
#endif

#ifdef ZBUFFER8
typedef struct tag_PingoDepth {
    uint8_t d;
} PingoDepth;
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Writes a depth value to the depth buffer.
 * 
 * @param d Pointer to the depth buffer.
 * @param idx Index at which to write the depth value.
 * @param value Depth value to write.
 */
void depth_write(PingoDepth *d, int idx, float value);

/**
 * @brief Checks if a given depth value is less than the value at a given index in the depth buffer.
 * 
 * @param d Pointer to the depth buffer.
 * @param idx Index at which to check the depth value.
 * @param value Depth value to compare.
 * @return True if the given depth value is less than the value at the index, false otherwise.
 */
bool depth_check(PingoDepth *d, int idx, float value);

#ifdef __cplusplus
}
#endif

