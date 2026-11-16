#include "Utils.h"

#include <cstdlib>
#include <iostream>
#include <sstream>

#include "Config.h"

SDL_Rect MakeRect(int x, int y, int w, int h) {
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;
    return r;
}

SDL_Rect ScreenRect() {
    return MakeRect(0, 0, kWindowWidth, kWindowHeight);
}

bool PointInRect(int x, int y, const SDL_Rect& r) {
    return x >= r.x && x < r.x + r.w &&
           y >= r.y && y < r.y + r.h;
}

int AtLeastOne(int value) {
    return value < 1 ? 1 : value;
}

int RoundToInt(float value) {
    return static_cast<int>(value + 0.5f);
}

int RandomRange(int lo, int hi) {
    return lo + std::rand() % (hi - lo + 1);
}

void LogWarning(const std::string& message) {
    std::cerr << "[warning] " << message << std::endl;
}

std::string AssetPath(const char* fileName) {
    return std::string(kAssetDir) + fileName;
}

std::string HourLabel(int hour) {
    std::ostringstream stream;
    stream << hour << " AM";
    return stream.str();
}