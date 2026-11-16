// Confetti.h - a single confetti particle.
#ifndef CONFETTI_H
#define CONFETTI_H

#include <SDL.h>

struct Confetti {
    float     x, y;
    float     vx, vy;
    SDL_Color color;
    int       size;
};

#endif // CONFETTI_H