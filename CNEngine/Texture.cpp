#include "Texture.h"

#include <SDL_image.h>

#include "Utils.h"

Texture::Texture() : texture_(NULL), width_(0), height_(0) {}

Texture::~Texture() {
    Reset();
}

bool Texture::LoadSvg(SDL_Renderer* renderer, const std::string& path,
                      float scale, float pixelScale) {
    Reset();

    SDL_RWops* rw = SDL_RWFromFile(path.c_str(), "rb");
    if (!rw) {
        LogWarning("Cannot open " + path + ": " + SDL_GetError());
        return false;
    }

    SDL_Surface* surface = IMG_LoadSVG_RW(rw);
    int logicalW = 0;
    int logicalH = 0;

    if (surface) {
        logicalW = AtLeastOne(static_cast<int>(surface->w * scale));
        logicalH = AtLeastOne(static_cast<int>(surface->h * scale));
        const int pixelW = AtLeastOne(RoundToInt(surface->w * scale * pixelScale));
        const int pixelH = AtLeastOne(RoundToInt(surface->h * scale * pixelScale));

        if (pixelW != surface->w || pixelH != surface->h) {
            SDL_FreeSurface(surface);
            SDL_RWseek(rw, 0, RW_SEEK_SET);
            surface = IMG_LoadSizedSVG_RW(rw, pixelW, pixelH);
        }
    }
    SDL_RWclose(rw);

    if (!surface) {
        LogWarning("Cannot load SVG " + path + ": " + IMG_GetError());
        return false;
    }
    return Adopt(renderer, surface, logicalW, logicalH);
}

bool Texture::LoadSvgFitted(SDL_Renderer* renderer, const std::string& path,
                            int logicalW, int logicalH, float pixelScale) {
    Reset();

    SDL_RWops* rw = SDL_RWFromFile(path.c_str(), "rb");
    if (!rw) {
        LogWarning("Cannot open " + path + ": " + SDL_GetError());
        return false;
    }

    const int pixelW = AtLeastOne(RoundToInt(logicalW * pixelScale));
    const int pixelH = AtLeastOne(RoundToInt(logicalH * pixelScale));
    SDL_Surface* surface = IMG_LoadSizedSVG_RW(rw, pixelW, pixelH);
    SDL_RWclose(rw);

    if (!surface) {
        LogWarning("Cannot load SVG " + path + ": " + IMG_GetError());
        return false;
    }
    return Adopt(renderer, surface, logicalW, logicalH);
}

bool Texture::CreateFromText(SDL_Renderer* renderer, TTF_Font* font,
                             const std::string& text, SDL_Color color, float pixelScale) {
    Reset();
    if (!font) {
        return false;
    }

    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
    if (!surface) {
        LogWarning(std::string("Text render error: ") + TTF_GetError());
        return false;
    }

    const int logicalW = AtLeastOne(RoundToInt(surface->w / pixelScale));
    const int logicalH = AtLeastOne(RoundToInt(surface->h / pixelScale));
    return Adopt(renderer, surface, logicalW, logicalH);
}

void Texture::Reset() {
    if (texture_) {
        SDL_DestroyTexture(texture_);
        texture_ = NULL;
    }
    width_ = 0;
    height_ = 0;
}

void Texture::SetAlpha(Uint8 alpha) const {
    if (texture_) {
        SDL_SetTextureAlphaMod(texture_, alpha);
    }
}

void Texture::Draw(SDL_Renderer* renderer, const SDL_Rect& dst, double angleDegrees) const {
    if (!texture_) return;
    if (angleDegrees == 0.0) {
        SDL_RenderCopy(renderer, texture_, NULL, &dst);
    } else {
        SDL_RenderCopyEx(renderer, texture_, NULL, &dst, angleDegrees, NULL, SDL_FLIP_NONE);
    }
}

void Texture::DrawAt(SDL_Renderer* renderer, int x, int y) const {
    Draw(renderer, MakeRect(x, y, width_, height_));
}

bool Texture::Adopt(SDL_Renderer* renderer, SDL_Surface* surface, int logicalW, int logicalH) {
    texture_ = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture_) {
        width_  = logicalW;
        height_ = logicalH;
    } else {
        LogWarning(std::string("SDL_CreateTextureFromSurface failed: ") + SDL_GetError());
    }
    SDL_FreeSurface(surface);
    return texture_ != NULL;
}