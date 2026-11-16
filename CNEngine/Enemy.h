// Enemy.h - a clickable enemy: alive sprite, shot sprite, optional overlay.
#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>

#include "Texture.h"

enum EnemyId {
    ENEMY_EGG,
    ENEMY_BUDDY,
    ENEMY_CLOWN,
    ENEMY_COUNT
};

struct Enemy {
    Texture alive;
    Texture shotSprite;
    Texture overlay;

    int    spawnX;
    int    spawnY;
    int    shotOffsetX;
    int    shotOffsetY;
    double shotAngle;

    SDL_Rect rect;
    bool     isVisible;
    bool     isShot;
    Uint32   shotTime;

    Enemy();

    void Reset();
    bool CanBeHit(int x, int y) const;
    void MarkShot(Uint32 now);
    void Draw(SDL_Renderer* renderer) const;

private:
    Enemy(const Enemy&);
    Enemy& operator=(const Enemy&);
};

#endif // ENEMY_H