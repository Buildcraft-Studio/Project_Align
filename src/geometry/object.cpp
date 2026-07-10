/** *@file object.cpp
 *  @brief Implements Mesh2. addSquare/addCircle are stubs pending real
 *         triangulation/tessellation logic.
**/
#include "geometry/object.hpp"
#include <algorithm>
#include <cmath>

namespace Engine::Geometry {

void Mesh2::addVertex(const Vector2& pos, const Vector2& norm, const Vector2& uv) {
    positions.push_back(pos);
    normals.push_back(norm);
    uvs.push_back(uv);
}

void Mesh2::addSquare(int a0, int a1, int a2) {
    // Emit a simple triangle using the supplied corner references. The mesh
    // can later be upgraded to a full quad topology when the engine needs it.
    if (a0 < 0 || a1 < 0 || a2 < 0) {
        return;
    }

    idex.push_back(a0);
    idex.push_back(a1);
    idex.push_back(a2);
}

void Mesh2::addCircle(int radius, int segmentCount) {
    if (segmentCount < 3) {
        segmentCount = 3;
    }

    const float radiusValue = static_cast<float>(std::max(1, radius));
    const float pi = 3.14159265358979323846f;
    const float step = 2.0f * pi / static_cast<float>(segmentCount);

    addVertex(Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f), Vector2(0.5f, 0.5f));

    for (int i = 0; i < segmentCount; ++i) {
        const float angle = step * static_cast<float>(i);
        addVertex(
            Vector2(std::cos(angle) * radiusValue, std::sin(angle) * radiusValue),
            Vector2(std::cos(angle), std::sin(angle)),
            Vector2(0.5f + std::cos(angle) * 0.5f, 0.5f + std::sin(angle) * 0.5f));
    }

    for (int i = 1; i <= segmentCount; ++i) {
        idex.push_back(0);
        idex.push_back(i);
        idex.push_back((i % segmentCount) + 1);
    }
}

}
