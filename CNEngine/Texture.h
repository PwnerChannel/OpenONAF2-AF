// Texture.h - RAII wrapper around SDL_Texture (SVG images and rendered text).
#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL.h>
#include <SDL_ttf.h>

#include <string>

class Texture {
public:
    Texture();
    ~Texture();

    bool LoadSvg(SDL_Renderer* renderer, const std::string& path,
                 float scale, float pixelScale);

    bool LoadSvgFitted(SDL_Renderer* renderer, const std::string& path,
                       int logicalW, int logicalH, float pixelScale);

    bool CreateFromText(SDL_Renderer* renderer, TTF_Font* font,
                        const std::string& text, SDL_Color color, float pixelScale);

    void Reset();

    bool IsValid() const { return texture_ != NULL; }
    int  Width()   const { return width_; }
    int  Height()  const { return height_; }

    // 0 = invisible, 255 = fully opaque. Affects all later draws of this texture.
    void SetAlpha(Uint8 alpha) const;

    void Draw(SDL_Renderer* renderer, const SDL_Rect& dst, double angleDegrees = 0.0) const;
    void DrawAt(SDL_Renderer* renderer, int x, int y) const;

private:
    Texture(const Texture&);
    Texture& operator=(const Texture&);

    bool Adopt(SDL_Renderer* renderer, SDL_Surface* surface, int logicalW, int logicalH);

    SDL_Texture* texture_;
    int          width_;
    int          height_;
};

#endif // TEXTURE_H