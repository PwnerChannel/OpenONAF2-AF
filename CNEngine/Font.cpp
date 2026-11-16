#include "Font.h"

#include <cstddef>
#include <stdexcept>

#include "Config.h"
#include "Utils.h"

Font::Font() : font_(NULL) {}

Font::~Font() {
    Close();
}

bool Font::Open(const char* path, int pointSize) {
    Close();
    font_ = TTF_OpenFont(path, pointSize);
    return font_ != NULL;
}

void Font::Close() {
    if (font_) {
        TTF_CloseFont(font_);
        font_ = NULL;
    }
}

void OpenUiFont(Font& font, int logicalPointSize, float pixelScale) {
    const int pointSize = AtLeastOne(RoundToInt(logicalPointSize * pixelScale));
    for (std::size_t i = 0; i < kFontCandidateCount; ++i) {
        if (font.Open(kFontCandidates[i], pointSize)) {
            return;
        }
    }
    throw std::runtime_error(
        "Could not open any UI font. Put a TrueType font at Data/font.ttf.");
}