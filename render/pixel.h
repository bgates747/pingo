#ifndef PIXEL_H
#define PIXEL_H

#include <stdint.h>
#include <stdlib.h>

// What format to use [ UINT8 | RGB565 | RGBA8888 | BGRA8888 | RGB888 ]
#define BGRA8888

// Formats definitions:
#ifdef UINT8
typedef struct Pixel {
    uint8_t g;
} Pixel;
#define PIXELBLACK (Pixel){0}
#define PIXELWHITE (Pixel){255}
#endif

#ifdef RGB565
typedef struct Pixel {
    uint8_t red:5;
    uint8_t green:6;
    uint8_t blue:5;
} Pixel;
#define PIXELBLACK (Pixel){0}
#define PIXELWHITE (Pixel){255}
#endif

#ifdef RGB888
typedef struct Pixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Pixel;

#define PIXELBLACK (Pixel){0,0,0}
#define PIXELWHITE (Pixel){255,255,255}
#endif

#ifdef RGBA8888
typedef struct Pixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} Pixel;

#define PIXELBLACK (Pixel){0,0,0,255}
#define PIXELWHITE (Pixel){255,255,255,255}
#endif

#ifdef BGRA8888
typedef struct Pixel {
    uint8_t b;
    uint8_t g;
    uint8_t r;
    uint8_t a;
} Pixel;

#define PIXELBLACK (Pixel){0,0,0,255}
#define PIXELWHITE (Pixel){255,255,255,255}
#endif

// Interface 

/**
 * @brief Generates a random pixel.
 * 
 * @return A Pixel structure with random color values.
 */
extern Pixel pixelRandom();

/**
 * @brief Creates a Pixel structure from a single 8-bit value.
 * 
 * @param value The 8-bit value.
 * @return The resulting Pixel structure.
 */
extern Pixel pixelFromUInt8(uint8_t value);

/**
 * @brief Converts a Pixel structure to a single 8-bit value.
 * 
 * @param pixel The Pixel structure.
 * @return The 8-bit value representing the pixel.
 */
extern uint8_t pixelToUInt8(Pixel *pixel);

/**
 * @brief Creates a Pixel structure from RGBA values.
 * 
 * @param r Red component.
 * @param g Green component.
 * @param b Blue component.
 * @param a Alpha component.
 * @return The resulting Pixel structure.
 */
extern Pixel pixelFromRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

/**
 * @brief Multiplies a Pixel structure by a floating-point factor.
 * 
 * @param pixel The Pixel structure.
 * @param factor The multiplication factor.
 * @return The resulting Pixel structure.
 */
extern Pixel pixelMul(Pixel pixel, float factor);

#endif // PIXEL_H
