#ifndef SCREEN_RENDERING_HPP
#define SCREEN_RENDERING_HPP

#include <array>
#include <string>

namespace Engine::Rendering {

    enum class RenderingMode {
        Windowed = 0,
        Fullscreen = 1
    };

    enum class WindowsResolution {
        Res720p = 0,
        Res1080p = 1,
        Res1440p = 2,
        Res4k = 3
    };

    struct Resolution {
        int width;
        int height;
    };

    struct RenderingSettings {
        RenderingMode mode;
        Resolution resolution;
    };

    struct RenderCanvas {
        int width;
        int height;
        std::array<int, 8> renderLayers;
        int currentLayer;
    };

    class ScreenRendering {
    public:
        ScreenRendering();

        void setRenderingMode(RenderingMode mode);
        void setWindowsResolution(WindowsResolution resolution);
        void setCanvasSize(int width, int height);
        void setCurrentLayer(int layer);
        void clearLayer(int layer);
        void drawPixel(int layer, int x, int y, int value);

        RenderingSettings getSettings() const;
        RenderCanvas getCanvas() const;
        std::string describe() const;

        void beginFrame();
        void endFrame();

    private:
        RenderingSettings settings_;
        RenderCanvas canvas_;
    };
}
#endif // SCREEN_RENDERING_HPP
