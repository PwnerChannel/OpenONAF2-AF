#include "Enemy.h"
#include "Utils.h"
#include <SDL.h>

Enemy::Enemy()
    : spawnX(0), spawnY(0), shotOffsetX(0), shotOffsetY(0), shotAngle(0.0),
      rect(MakeRect(0, 0, 0, 0)), isVisible(false), isShot(false), shotTime(0) {}

void Enemy::Reset() {
    rect = MakeRect(spawnX, spawnY, alive.Width(), alive.Height());
    isVisible = false;
    isShot = false;
    shotTime = 0;
}

bool Enemy::CanBeHit(int x, int y) const {
    return isVisible && !isShot && PointInRect(x, y, rect);
}

void Enemy::MarkShot(Uint32 now) {
    isShot = true;
    shotTime = now;
    
    // Больше не меняем координаты здесь наглухо — 
    // теперь позиция будет плавно доезжать в Draw()
    if (shotSprite.IsValid()) {
        rect.w = shotSprite.Width();
        rect.h = shotSprite.Height();
    }
}

void Enemy::Draw(SDL_Renderer* renderer) const {
    if (!isVisible) return;

    SDL_Rect currentRect = rect;

    // Если враг убит, плавно сдвигаем его в течение 120 миллисекунд
    if (isShot) {
        Uint32 elapsed = SDL_GetTicks() - shotTime;
        float progress = static_cast<float>(elapsed) / 120.0f;
        if (progress > 1.0f) {
            progress = 1.0f;
        }

        // Линейный плавный слайд на долю от shotOffsetX / shotOffsetY
        currentRect.x += static_cast<int>(shotOffsetX * progress);
        currentRect.y += static_cast<int>(shotOffsetY * progress);
    }

    if (!isShot) {
        alive.Draw(renderer, currentRect);
        return;
    }

    if (shotSprite.IsValid()) {
        shotSprite.Draw(renderer, currentRect);
    } else {
        alive.Draw(renderer, currentRect, shotAngle);
    }

    if (overlay.IsValid()) {
        overlay.DrawAt(renderer,
                       currentRect.x + (currentRect.w - overlay.Width())  / 2,
                       currentRect.y + (currentRect.h - overlay.Height()) / 2);
    }
}