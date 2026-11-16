#include "VhsEffect.h"
#include "Config.h"
#include "Utils.h"

namespace {

// Детерминированный хэш для стабильного мерцания
Uint32 Hash(Uint32 x) {
    x ^= x >> 16;
    x *= 0x7feb352dU;
    x ^= x >> 15;
    x *= 0x846ca68bU;
    x ^= x >> 16;
    return x;
}

int ScaleAlpha(int value, int alpha) {
    return value * alpha / 255;
}

void FillRectAlpha(SDL_Renderer* renderer, const SDL_Rect& r,
                   int red, int green, int blue, int a) {
    SDL_SetRenderDrawColor(renderer,
                           static_cast<Uint8>(red),
                           static_cast<Uint8>(green),
                           static_cast<Uint8>(blue),
                           static_cast<Uint8>(a));
    SDL_RenderFillRect(renderer, &r);
}

} // namespace

void VhsEffect::Draw(SDL_Renderer* renderer, const Texture& text,
                     int x, int y, int alpha, Uint32 t) const {
    if (alpha <= 0) {
        return;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // ---- Сам текст (оставляем чистым) ----
    text.SetAlpha(static_cast<Uint8>(alpha));
    text.DrawAt(renderer, x, y);

    // ---- Замедленные и улучшенные ретро-полосы ----
    // Замедлили смену кадров (теперь 220 вместо 90), чтобы полосы не мельтешили слишком быстро
    const Uint32 barFrame = t / 220; 

    // Количество одновременных полос (1 или 2 для аккуратного вида)
    const int numBars = 1 + static_cast<int>(Hash(barFrame) % 2u);

    for (int i = 0; i < numBars; ++i) {
        const Uint32 h = Hash(barFrame * 97u + static_cast<Uint32>(i) * 211u);
        
        // Случайная вертикальная позиция полосы
        const int barY = static_cast<int>(h % static_cast<Uint32>(kWindowHeight));
        
        // Более выразительная высота полос (от 30 до 160 пикселей)
        int barHeight = 30 + static_cast<int>((h >> 8) % 130u);
        
        if (barY + barHeight > kWindowHeight) {
            barHeight = kWindowHeight - barY;
        }

        // Базовая яркость/прозрачность полосы
        const int baseAlpha = 180 + static_cast<int>((h >> 16) % 75u);
        
        // Дополнительное легкое покадровое мерцание для «живого» аналогового эффекта
        const int flicker = (t / 60 + i) % 2 == 0 ? 255 : baseAlpha;

        FillRectAlpha(renderer, MakeRect(0, barY, kWindowWidth, barHeight),
                      255, 255, 255, ScaleAlpha(flicker, alpha));
    }

    text.SetAlpha(255);
}