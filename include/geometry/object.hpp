/** *@file object.hpp
 *  @brief Declares core 2D/3D geometry primitives: vectors and meshes.
**/
#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <vector>

namespace Engine::Geometry {

    /// @brief A 3D vector.
    struct Vector3 {
        float x;
        float y;
        float z;

        Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f)
            : x(x), y(y), z(z) {}
    };

    /// @brief A 2D vector.
    struct Vector2 {
        float x;
        float y;

        Vector2(float x = 0.0f, float y = 0.0f)
            : x(x), y(y) {}
    };

    /** *@brief Classifies a Mesh2's shape/behavior.
     *  @details Square/Circle are static and can't deform - used for hit
     *           detection. DMesh is a deformable mesh whose geometry can
     *           change (e.g. via a Modifier).
    **/
    enum class Mesh_Type {
        Square = 0, ///< Static square - hit detection only, can't deform.
        Circle = 1, ///< Static circle - hit detection only, can't deform.
        DMesh  = 2  ///< Deformable mesh - geometry can change at runtime.
    };

    /** *@class Mesh2
     *  @brief A 2D mesh; will be commonly used for both rendering geometry
     *         and hit-detection shapes.
    **/
    class Mesh2
    {
        public:
            std::vector<Vector2> positions; ///< Vertex positions.
            std::vector<Vector2> normals;   ///< Per-vertex normals.
            std::vector<Vector2> uvs;       ///< Per-vertex UV coordinates.
            std::vector<int>     idex;      ///< Index buffer (triangle/face indices).
            int                  type = 0;  ///< Cast from Mesh_Type - see enum above.

            /** *@brief Append one vertex (position + normal + UV) to the mesh.
             *  @param pos  Vertex position.
             *  @param norm Vertex normal.
             *  @param uv   Vertex UV coordinate.
            **/
            void addVertex(const Vector2& pos, const Vector2& norm, const Vector2& uv);

            /** *@brief Build a square from two corner-index references.
             *  @param a0 First corner vertex index.
             *  @param a1 Second corner vertex index.
             *  @param a2 Third corner vertex index.
             *  @note Parameter names were previously invalid (`int int, ...`
             *        used a reserved keyword as an identifier) - fixed here.
             *        Still a STUB: no triangulation logic yet.
            **/
            void addSquare(int a0, int a1, int a2);

            /** *@brief Build a circle approximation from a radius and a
             *         segment count.
             *  @param radius       Circle radius (previously an untyped/invalid
             *                      `int int` parameter - fixed here).
             *  @param segmentCount Number of segments to approximate the circle with.
             *  @note STUB: no tessellation logic yet.
            **/
            void addCircle(int radius, int segmentCount);
    };

    /// @brief 3D counterpart to Mesh2. STUB: currently just a copy shell -
    ///        add triangle/quad/n-gon support here.
    class Mesh3 {

    };
}
#endif // OBJECT_HPP
