#include "pixel.h"

#ifdef UINT8

/**
 * @brief Generates a random pixel with an 8-bit grayscale value.
 * 
 * @return A Pixel structure with a random grayscale value.
 */
extern Pixel pixelRandom() {
    return (Pixel){(uint8_t)rand()};
}

/**
 * @brief Converts a Pixel structure to a single 8-bit value.
 * 
 * @param p The Pixel structure.
 * @return The 8-bit grayscale value.
 */
uint8_t pixelToUInt8(Pixel *p) {
    return p->g;
}

/**
 * @brief Creates a Pixel structure from a single 8-bit value.
 * 
 * @param g The 8-bit value.
 * @return The resulting Pixel structure.
 */
extern Pixel pixelFromUInt8(uint8_t g) {
    return (Pixel){g};
}

/**
 * @brief Multiplies a Pixel structure by a floating-point factor.
 * 
 * @param p The Pixel structure.
 * @param f The multiplication factor.
 * @return The resulting Pixel structure.
 */
extern Pixel pixelMul(Pixel p, float f) {
    return (Pixel){(uint8_t)(p.g * f)};
}

/**
 * @brief Creates a Pixel structure from RGBA values, converting to grayscale.
 * 
 * @param r Red component.
 * @param g Green component.
 * @param b Blue component.
 * @param a Alpha component.
 * @return The resulting Pixel structure.
 */
extern Pixel pixelFromRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return (Pixel){(uint8_t)((r + g + b) / 3)};
}
#endif

#ifdef RGB888

/**
 * @brief Generates a random pixel with RGB values.
 * 
 * @return A Pixel structure with random RGB values.
 */
extern Pixel pixelRandom() {
    return (Pixel){(uint8_t)rand(), (uint8_t)rand(), (uint8_t)rand()};
}

/**
 * @brief Converts a Pixel structure to a 32-bit RGBA value.
 * 
 * @param p The Pixel structure.
 * @return The 32-bit RGBA value.
 */
uint32_t pixelToRGBA(Pixel *p) {
    return (uint32_t)(p->r | p->g << 8 | p->b << 16 | 255 << 24);
}
#endif

#ifdef RGBA8888

/**
 * @brief Generates a random pixel with RGBA values.
 * 
 * @return A Pixel structure with random RGBA values.
 */
extern Pixel pixelRandom() {
    return (Pixel){(uint8_t)rand(), (uint8_t)rand(), (uint8_t)rand(), 255};
}

/**
 * @brief Creates a Pixel structure from a single 8-bit value.
 * 
 * @param g The 8-bit value.
 * @return The resulting Pixel structure.
 */
extern Pixel pixelFromUInt8(uint8_t g) {
    return (Pixel){g, g, g, 255};
}

/**
 * @brief Converts a Pixel structure to a single 8-bit value.
 * 
 * @param p The Pixel structure.
 * @return The 8-bit grayscale value.
 */
extern uint8_t pixelToUInt8(Pixel *p) {
    return (uint8_t)((p->r + p->g + p->b) / 3);
}

/**
 * @brief Creates a Pixel structure from RGBA values.
 * 
 * @param r Red component.
 * @param g Green component.
 * @param b Blue component.
 * @param a Alpha component.
 * @return The resulting Pixel structure.
 */
extern Pixel pixelFromRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return (Pixel){r, g, b, a};
}

/**
 * @brief Multiplies a Pixel structure by a floating-point factor.
 * 
 * @param p The Pixel structure.
 * @param f The multiplication factor.
 * @return The resulting Pixel structure.
 */
extern Pixel pixelMul(Pixel p, float f) {
    return (Pixel){(uint8_t)(p.r * f), (uint8_t)(p.g * f), (uint8_t)(p.b * f), p.a};
}

#endif

#ifdef BGRA8888

/**
 * @brief Generates a random pixel with BGRA values.
 * 
 * @return A Pixel structure with random BGRA values.
 */
extern Pixel pixelRandom() {
    return (Pixel){(uint8_t)rand(), (uint8_t)rand(), (uint8_t)rand(), 255};
}

/**
 * @brief Creates a Pixel structure from a single 8-bit value.
 * 
 * @param g The 8-bit value.
 * @return The resulting Pixel structure.
 */
extern Pixel pixelFromUInt8(uint8_t g) {
    return (Pixel){g, g, g, 255};
}

/**
 * @brief Converts a Pixel structure to a single 8-bit value.
 * 
 * @param p The Pixel structure.
 * @return The 8-bit grayscale value.
 */
extern uint8_t pixelToUInt8(Pixel *p) {
    return (uint8_t)((p->r + p->g + p->b) / 3);
}

/**
 * @brief Creates a Pixel structure from RGBA values.
 * 
 * @param r Red component.
 * @param g Green component.
 * @param b Blue component.
 * @param a Alpha component.
 * @return The resulting Pixel structure.
 */
extern Pixel pixelFromRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return (Pixel){b, g, r, a};
}

/**
 * @brief Multiplies a Pixel structure by a floating-point factor.
 * 
 * @param p The Pixel structure.
 * @param f The multiplication factor.
 * @return The resulting Pixel structure.
 */
extern Pixel pixelMul(Pixel p, float f) {
    return (Pixel){(uint8_t)(p.b * f), (uint8_t)(p.g * f), (uint8_t)(p.r * f), p.a};
}

#endif
