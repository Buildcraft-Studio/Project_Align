/** *@file modifier.cpp
 *  @brief Implements Modifier. apply() is an intentionally empty stub -
 *         see class docs; subclass and override for a real operation.
**/
#include "geometry/modifier.hpp"

namespace Engine::Geometry {

Modifier::Modifier() {

}

Modifier::~Modifier() {

}

void Modifier::apply(Mesh2& target) {
    // TODO: no behavior yet - this is an intentionally empty stub. A
    // concrete modifier (extrude, bevel, subdivide, ...) should override
    // this to mutate `target` in place.
    (void)target;
}

}
