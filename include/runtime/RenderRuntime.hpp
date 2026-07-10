/** *@file RenderRuntime.hpp
 *  @brief Declares RenderRuntime, the runtime module responsible for
 *         drawing/presenting a frame each tick.
 *  @ingroup RuntimeSystem
**/
#ifndef RENDERRUNTIME_HPP
#define RENDERRUNTIME_HPP

#include "runtime/runtimeManger.hpp"

namespace Engine::Runtime {

    /** *@class RenderRuntime
     *  @brief Drives frame rendering. Implements IRuntimeModule so it gets
     *         a WatchDog'd table entry - if rendering stalls (e.g. a hung
     *         draw call), it'll show up as STALED/DEAD the same as any
     *         other managed runtime. Register through Bootstrap.
     *  @ingroup RuntimeSystem
    **/
    class RenderRuntime : public IRuntimeModule
    {
        public:
            /** *@brief Construct with this module's own runtime ID.
             *  @param id The runtime ID this module will be registered under.
            **/
            explicit RenderRuntime(int16_t id);
            virtual ~RenderRuntime();

            /** *@brief IRuntimeModule contract - reports its own ID during WatchDog().
             *  @return This module's own runtime ID.
            **/
            int16_t getRuntimeID() const override;

            /** *@brief Draw one frame.
             *  @note STUB - no actual graphics API (OpenGL/Vulkan/etc.) call
             *        is wired in yet. This is the entry point where the real
             *        draw-call submission belongs. Call incrementCounter()
             *        for this runtime's ID (via RuntimeManger) each time this
             *        runs, so WatchDog() can track that frames are still
             *        being produced.
            **/
            void renderFrame();

        private:
            int16_t id; ///< This module's own runtime ID.
    };
}
#endif // RENDERRUNTIME_HPP
