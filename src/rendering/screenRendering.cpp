#include "rendering/screenRendering.hpp"
#include <sstream>

namespace Engine::Rendering {

ScreenRendering::ScreenRendering()
    : settings_({RenderingMode::Windowed, {1280, 720}}), canvas_({1280, 720, {}, 0}) {
    canvas_.renderLayers.fill(0);
}

void ScreenRendering::setRenderingMode(RenderingMode mode) {
    settings_.mode = mode;
}

void ScreenRendering::setWindowsResolution(WindowsResolution resolution) {
    switch (resolution) {
        case WindowsResolution::Res720p:
            settings_.resolution = {1280, 720};
            break;
        case WindowsResolution::Res1080p:
            settings_.resolution = {1920, 1080};
            break;
        case WindowsResolution::Res1440p:
            settings_.resolution = {2560, 1440};
            break;
        case WindowsResolution::Res4k:
            settings_.resolution = {3840, 2160};
            break;
    }
    canvas_.width = settings_.resolution.width;
    canvas_.height = settings_.resolution.height;
}

void ScreenRendering::setCanvasSize(int width, int height) {
    canvas_.width = width;
    canvas_.height = height;
    settings_.resolution.width = width;
    settings_.resolution.height = height;
}

void ScreenRendering::setCurrentLayer(int layer) {
    if (layer >= 0 && layer < static_cast<int>(canvas_.renderLayers.size())) {
        canvas_.currentLayer = layer;
    }
}

void ScreenRendering::clearLayer(int layer) {
    if (layer >= 0 && layer < static_cast<int>(canvas_.renderLayers.size())) {
        canvas_.renderLayers[static_cast<std::size_t>(layer)] = 0;
    }
}

void ScreenRendering::drawPixel(int layer, int x, int y, int value) {
    if (layer < 0 || layer >= static_cast<int>(canvas_.renderLayers.size())) {
        return;
    }

    if (x < 0 || y < 0 || x >= canvas_.width || y >= canvas_.height) {
        return;
    }

    canvas_.renderLayers[static_cast<std::size_t>(layer)] += value;
}

RenderingSettings ScreenRendering::getSettings() const {
    return settings_;
}

RenderCanvas ScreenRendering::getCanvas() const {
    return canvas_;
}

std::string ScreenRendering::describe() const {
    std::ostringstream stream;
    stream << "mode=" << (settings_.mode == RenderingMode::Fullscreen ? "fullscreen" : "windowed")
           << " res=" << settings_.resolution.width << "x" << settings_.resolution.height
           << " layer=" << canvas_.currentLayer;
    return stream.str();
}

void ScreenRendering::beginFrame() {
    canvas_.renderLayers.fill(0);
}

void ScreenRendering::endFrame() {
    canvas_.renderLayers[static_cast<std::size_t>(canvas_.currentLayer)] = 1;
}

}
