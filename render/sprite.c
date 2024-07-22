#include "sprite.h"
#include "../math/mat3.h"

/**
 * @brief Initializes a sprite.
 * 
 * @param s Pointer to the sprite to initialize.
 * @param f Texture frame for the sprite.
 * @param t Transformation matrix for the sprite.
 * @return 0 on success, 1 if the framebuffer is null.
 */
int spriteInit(Sprite *s, Texture f, Mat4 t) {
    if (f.framebuffer == 0)
        return 1;

    s->frame = f;
    s->t = t;

    return 0;
}

/**
 * @brief Converts a sprite to a renderable object.
 * 
 * @param s Pointer to the sprite to convert.
 * @return The resulting renderable object.
 */
Renderable spriteAsRenderable(Sprite *s) {
    return (Renderable){.renderableType = RENDERABLE_SPRITE, .impl = s};
}

/**
 * @brief Randomizes the pixels of the sprite's texture frame.
 * 
 * @param s Pointer to the sprite to randomize.
 * @return 0 on success.
 */
int spriteRandomize(Sprite *s) {
    for (int x = 0; x < s->frame.size.x; x++) {
        for (int y = 0; y < s->frame.size.y; y++) {
            texture_draw(&s->frame, (Vec2i){x, y}, pixelRandom());
        }
    }

    return 0;
}
