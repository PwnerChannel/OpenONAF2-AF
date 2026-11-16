// VhsEffect.h - light VHS interference look (used on the intro screen).
//
// Effects: RGB split (chromatic ghosting), per-row horizontal jitter, short
// glitch bursts, a slow rolling distortion bar, scanlines and static streaks.
// Tweak the strength in Config.h (kVhs... constants).
#ifndef VHSEFFECT_H
#define VHSEFFECT_H

#include <SDL.h>

#include "Texture.h"

class VhsEffect {
public:
    // Draws `text` with its top-left at (x, y) plus the screen-wide overlay
    // effects. `alpha` (0..255) scales everything so it fades out with the
    // black screen. `timeMs` is time since the effect started.
    void Draw(SDL_Renderer* renderer, const Texture& text,
              int x, int y, int alpha, Uint32 timeMs) const;
};

#endif // VHSEFFECT_H