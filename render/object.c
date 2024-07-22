#include "object.h"

/**
 * @brief Converts an object to a renderable object.
 * 
 * @param object Pointer to the object to convert.
 * @return The resulting renderable object.
 */
Renderable object_as_renderable(Object *object) {
    return (Renderable){.renderableType = RENDERABLE_OBJECT, .impl = object};
}
