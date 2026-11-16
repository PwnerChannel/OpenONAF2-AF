// Graphics.h - owns the SDL window and renderer (with HiDPI handling).
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL.h>

class Graphics {
public:
    Graphics(const char* title, int width, int height);
    ~Graphics();

    SDL_Renderer* GetRenderer() const { return renderer_; }
    bool  HasVsync() const { return vsync_; }
    float GetPixelScale() const { return pixelScale_; }

private:
    Graphics(const Graphics&);
    Graphics& operator=(const Graphics&);

    SDL_Window*   window_;
    SDL_Renderer* renderer_;
    bool          vsync_;
    float         pixelScale_;
};

#endif // GRAPHICS_H