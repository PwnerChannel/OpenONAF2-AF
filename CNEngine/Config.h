// Config.h - all tweakable constants live here.
#ifndef CONFIG_H
#define CONFIG_H

#include <SDL.h>
#include <cstddef>

// ---- Window ----
const int  kWindowWidth   = 800;
const int  kWindowHeight  = 600;
const char kWindowTitle[] = "April Fools Game";

// ---- Assets ----
const char kAssetDir[] = "Data/Sprites/";

const char* const kFontCandidates[] = {
    "Data/times.ttf",
    "C:/Windows/Fonts/times.ttf"
};
const std::size_t kFontCandidateCount =
    sizeof(kFontCandidates) / sizeof(kFontCandidates[0]);

// ---- Font sizes ----
const int kTitleFontSize  = 48;
const int kMenuFontSize   = 28;
const int kClockFontSize  = 39;
const int kEndingFontSize = 64;
const int kIntroFontSize  = 40;

// ---- Colors ----
const SDL_Color kWhite         = { 255, 255, 255, 255 };
const SDL_Color kBlack         = {   0,   0,   0, 255 };
const SDL_Color kGold          = { 255, 215,   0, 255 };
const SDL_Color kDisabledColor = { 255, 255, 255, 255 };

// ---- Menu layout ----
const int kMenuLeftMargin = 80;
const int kTitleY         = 100;
const int kPlayY          = 250;
const int kDontPlayY      = 320;

// ---- HUD ----
const int kClockMargin = 20;

// ---- Enemies ----
const float  kEnemyScale        = 1.2f;
const Uint32 kEnemySpawnDelayMs = 3000;

// ---- Intro screen (shown after pressing Play, before the game starts) ----
const char   kIntroText[]       = "Survive until April Fools";
const Uint32 kIntroHoldMs       = 2500;   // fully black, text visible
const Uint32 kIntroFadeMs       = 1500;   // smooth fade-out time

// ---- VHS lines on the intro screen (lower values = subtler) ----
const int    kVhsScanlineSpacing  = 4;      // px between scanlines
const int    kVhsScanlineAlpha    = 40;     // 0..255
const int    kVhsNoiseCount       = 18;     // static streaks per update
const int    kVhsBarHeight        = 36;     // rolling bar height (px)
const int    kVhsBarAlpha         = 8;      // 0..255
const float  kVhsBarSpeed         = 0.10f;  // px per ms
const Uint32 kVhsBurstPeriodMs    = 2200;   // a flicker burst may start this often
const Uint32 kVhsBurstLengthMs    = 120;    // how long a burst lasts
const Uint32 kVhsBurstChance      = 45;     // % chance per period
const Uint32 kVhsStartBurstMs     = 150;    // flicker right as the screen appears
const int    kVhsTrackingAlpha    = 30;     // burst tracking lines, 0..255

// ---- Ending sequence ----
const Uint32 kFadeDurationMs    = 1000;
const Uint32 kClockPhaseEndMs   = 4500;
const Uint32 kEndingDurationMs  = 8000;
const int    kHourCount         = 6;
const int    kFinalLabelOffsetY = -30;

// ---- Confetti ----
const int   kConfettiCount   = 150;
const int   kConfettiMinSize = 5;
const int   kConfettiMaxSize = 10;
const float kLegacyFps       = 60.0f;

const SDL_Color kConfettiPalette[] = {
    { 255,   0, 127, 255 },
    {   0, 255, 255, 255 },
    { 255, 255,   0, 255 },
    {  50, 255,  50, 255 },
    { 255, 128,   0, 255 }
};
const int kConfettiPaletteSize =
    static_cast<int>(sizeof(kConfettiPalette) / sizeof(kConfettiPalette[0]));

// ---- Main loop ----
const float kMaxFrameSeconds = 0.1f;

#endif // CONFIG_H