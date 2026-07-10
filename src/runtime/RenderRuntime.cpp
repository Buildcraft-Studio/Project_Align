/** *@file RenderRuntime.cpp
 *  @brief Implements RenderRuntime. Frame drawing is a stub pending an
 *         actual graphics API being wired into the project.
 *  @ingroup RuntimeSystem
**/
#include "runtime/RenderRuntime.hpp"
#include <iostream>

namespace Engine::Runtime {

RenderRuntime::RenderRuntime(int16_t id)
    : id(id) {

}

RenderRuntime::~RenderRuntime() {

}

int16_t RenderRuntime::getRuntimeID() const {
    return id;
}

void RenderRuntime::renderFrame() {
    // TODO: replace with a real draw-call submission (OpenGL/Vulkan/etc.)
    std::cout << "[RenderRuntime] (stub) would render one frame\n";
}

}
