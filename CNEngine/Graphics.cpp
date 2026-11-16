#include "Graphics.h"

#include <stdexcept>
#include <string>

#include "Utils.h"

Graphics::Graphics(const char* title, int width, int height)
    : window_(NULL), renderer_(NULL), vsync_(false), pixelScale_(1.0f) {
    window_ = SDL_CreateWindow(title,
                               SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               width, height,
                               SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
    if (!window_) {
        throw std::runtime_error(std::string("SDL_CreateWindow failed: ") + SDL_GetError());
    }

    renderer_ = SDL_CreateRenderer(window_, -1,
                                   SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer_) {
        LogWarning(std::string("Accelerated renderer unavailable, using software: ") +
                   SDL_GetError());
        renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_SOFTWARE);
    }
    if (!renderer_) {
        const std::string message =
            std::string("SDL_CreateRenderer failed: ") + SDL_GetError();
        SDL_DestroyWindow(window_);
        throw std::runtime_error(message);
    }

    SDL_RendererInfo info;
    if (SDL_GetRendererInfo(renderer_, &info) == 0) {
        vsync_ = (info.flags & SDL_RENDERER_PRESENTVSYNC) != 0;
    }

    int windowW = 0, windowH = 0, outputW = 0, outputH = 0;
    SDL_GetWindowSize(window_, &windowW, &windowH);
    if (SDL_GetRendererOutputSize(renderer_, &outputW, &outputH) == 0 && windowW > 0) {
        pixelScale_ = static_cast<float>(outputW) / static_cast<float>(windowW);
    }
    if (pixelScale_ < 1.0f) {
        pixelScale_ = 1.0f;
    }
    SDL_RenderSetScale(renderer_, pixelScale_, pixelScale_);
}

Graphics::~Graphics() {
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
}