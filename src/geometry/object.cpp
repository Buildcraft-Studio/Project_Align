/** *@file object.cpp
 *  @brief Implements Mesh2. addSquare/addCircle are stubs pending real
 *         triangulation/tessellation logic.
**/
#include "geometry/object.hpp"

namespace Engine::Geometry {

void Mesh2::addVertex(const Vector2& pos, const Vector2& norm, const Vector2& uv) {
    positions.push_back(pos);
    normals.push_back(norm);
    uvs.push_back(uv);
}

void Mesh2::addSquare(int a0, int a1, int a2) {
    // TODO: no triangulation yet - this is where two triangles forming a
    // square would be pushed into `idex` based on already-added vertices.
    idex.push_back(a0);
    idex.push_back(a1);
    idex.push_back(a2);
}

void Mesh2::addCircle(int radius, int segmentCount) {
    // TODO: no tessellation yet - this is where `segmentCount` vertices
    // around `radius` would be generated via addVertex() and connected
    // into a triangle fan pushed into `idex`.
    (void)radius;
    (void)segmentCount;
}

}
