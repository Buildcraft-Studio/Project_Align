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
    // Apply a small translation to the mesh so the prototype demonstrates
    // that modifiers can mutate geometry in place.
    for (auto& position : target.positions) {
        position.x += 0.01f;
        position.y += 0.01f;
    }
}

}
