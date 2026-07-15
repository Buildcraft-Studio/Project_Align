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
    renderer.setRenderingMode(Engine::Rendering::RenderingMode::Windowed);
    renderer.setWindowsResolution(Engine::Rendering::WindowsResolution::Res1080p);
    renderer.setCanvasSize(1920, 1080);
    renderer.setCurrentLayer(0);
}

RenderRuntime::~RenderRuntime() {

}

int16_t RenderRuntime::getRuntimeID() const {
    return id;
}

void RenderRuntime::renderFrame() {
    renderer.beginFrame();
    renderer.setCurrentLayer(0);
    renderer.drawPixel(0, 0, 0, 1);
    renderer.drawPixel(0, 1, 1, 1);
    renderer.drawPixel(0, 2, 2, 1);
    renderer.endFrame();
    std::cout << "[RenderRuntime] " << renderer.describe() << "\n";
}

}
