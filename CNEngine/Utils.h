// Utils.h - small helper functions used across the project.
#ifndef UTILS_H
#define UTILS_H

#include <SDL.h>
#include <string>

SDL_Rect    MakeRect(int x, int y, int w, int h);
SDL_Rect    ScreenRect();
bool        PointInRect(int x, int y, const SDL_Rect& r);
int         AtLeastOne(int value);
int         RoundToInt(float value);
int         RandomRange(int lo, int hi);
void        LogWarning(const std::string& message);
std::string AssetPath(const char* fileName);
std::string HourLabel(int hour);

#endif // UTILS_H