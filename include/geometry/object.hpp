#ifndef OBJECT_HPP
#define OBJECT_HPP

#pragma once
#include <vector>
namespace Engine::Geometry {

    // Define a 3D vecter
    struct  Vector3 {
        float x;
        float y;
        float z;

        Vector3(float x = 0.0f, float y = 0.0f, z = 0.0f)
            : x(x), y(y), z(z) {}
    };

        // Define a 2D vecter
    struct  Vector2 {
        float x;
        float y;

        Vector2(float x = 0.0f, float y = 0.0f)
            : x(x), y(y) {}
    };

    enum class Mesh_Type {
        0 = Square,// These two are static and cant deform
        1 = Circle,// They are to be used to define hit detection

        2 = DMesh; // Defined a mesh that can change
    };
    class Mesh2 { // A 3D mesh // will be commonly used
        public:
                std::vector<Vector2> positions; // Define the orign of the Rectangle
                std::vector<Vector2> normals;
                std::vector<Vector2> uvs;
                std::vector<int> idex;
                int type;

                void addVertex(const Vector2& pos, const Vector2& norm, const Vector2& uv);
                void addSquare ( int a0, int a1, int ); // use to define square
                void addCircle ( int int, int i1); // used to define radias and the
    };
    class Mesh3 {}; // Just copy of Mesh2 but used. Add tringle, quad or n-gone

}

#endif // OBJECT_HPP
