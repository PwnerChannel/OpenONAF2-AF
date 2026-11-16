// Font.h - RAII wrapper around TTF_Font plus a helper that finds a UI font.
#ifndef FONT_H
#define FONT_H

#include <SDL_ttf.h>

class Font {
public:
    Font();
    ~Font();

    bool Open(const char* path, int pointSize);
    void Close();

    TTF_Font* Get() const { return font_; }

private:
    Font(const Font&);
    Font& operator=(const Font&);

    TTF_Font* font_;
};

// Tries each font in kFontCandidates; throws if none can be opened.
void OpenUiFont(Font& font, int logicalPointSize, float pixelScale);

#endif // FONT_H