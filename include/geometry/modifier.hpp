/** *@file modifier.hpp
 *  @brief Declares Modifier, a geometry-mutating operator applied to a Mesh.
**/
#ifndef MODIFIER_HPP
#define MODIFIER_HPP

#include "geometry/object.hpp"

namespace Engine::Geometry {

    /** *@class Modifier
     *  @brief Base type for an operation that mutates a Mesh2's geometry
     *         (e.g. extrude, bevel, subdivide). Add concrete subclasses per
     *         operation rather than growing this into one large switch.
    **/
    class Modifier
    {
        public:
            Modifier();
            virtual ~Modifier();

            /** *@brief Apply this modifier's operation to the given mesh, in place.
             *  @param target The mesh to mutate.
             *  @note STUB - no actual mutation logic yet. This is the entry
             *        point a concrete modifier (e.g. an ExtrudeModifier
             *        subclass) would override to do real work.
            **/
            virtual void apply(Mesh2& target);

        protected:

        private:
    };
}
#endif // MODIFIER_HPP
