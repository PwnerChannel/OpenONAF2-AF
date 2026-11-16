#include "SdlRuntime.h"

#include <SDL.h>
#include <SDL_ttf.h>

#include <stdexcept>
#include <string>

SdlRuntime::SdlRuntime() {
    SDL_SetHint("SDL_WINDOWS_DPI_AWARENESS", "permonitorv2");

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        throw std::runtime_error(std::string("SDL_Init failed: ") + SDL_GetError());
    }
    if (TTF_Init() != 0) {
        const std::string message = std::string("TTF_Init failed: ") + TTF_GetError();
        SDL_Quit();
        throw std::runtime_error(message);
    }
}

SdlRuntime::~SdlRuntime() {
    TTF_Quit();
    SDL_Quit();
}