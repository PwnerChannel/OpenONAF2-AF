#include "Game.h"

#include <cstddef>

#include "Config.h"
#include "Font.h"
#include "Utils.h"

// ---------------------------------------------------------------------------
// Construction / destruction
// ---------------------------------------------------------------------------

Game::Game()
    : runtime_(),
      graphics_(kWindowTitle, kWindowWidth, kWindowHeight),
      arrowCursor_(NULL),
      handCursor_(NULL),
      handCursorActive_(false),
      titleRect_(MakeRect(0, 0, 0, 0)),
      playRect_(MakeRect(0, 0, 0, 0)),
      dontPlayRect_(MakeRect(0, 0, 0, 0)),
      confettiSpawned_(false),
      state_(STATE_MENU),
      running_(true),
      introStartTime_(0),
      gameStartTime_(0),
      endingStartTime_(0) {
    LoadResources();

    arrowCursor_ = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    handCursor_  = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
}

Game::~Game() {
    SDL_ShowCursor(SDL_ENABLE);
    if (arrowCursor_) SDL_FreeCursor(arrowCursor_);
    if (handCursor_)  SDL_FreeCursor(handCursor_);
}

// ---------------------------------------------------------------------------
// Resource loading
// ---------------------------------------------------------------------------

void Game::LoadResources() {
    LoadMenuResources();
    LoadGameResources();
    LoadIntroResources();
    LoadEndingResources();
}

void Game::LoadMenuResources() {
    SDL_Renderer* renderer = graphics_.GetRenderer();
    const float pixelScale = graphics_.GetPixelScale();

    Font titleFont;
    Font bodyFont;
    OpenUiFont(titleFont, kTitleFontSize, pixelScale);
    OpenUiFont(bodyFont, kMenuFontSize, pixelScale);

    menuBackground_.LoadSvgFitted(renderer, AssetPath("menu_bg.svg"),
                                  kWindowWidth, kWindowHeight, pixelScale);

    titleText_.CreateFromText(renderer, titleFont.Get(), "Awesome April Fools Game", kWhite, pixelScale);
    playText_.CreateFromText(renderer, bodyFont.Get(), "Play Game", kWhite, pixelScale);
    dontPlayText_.CreateFromText(renderer, bodyFont.Get(), "Don't play the game", kDisabledColor, pixelScale);

    titleRect_    = MakeRect(kMenuLeftMargin, kTitleY,    titleText_.Width(),    titleText_.Height());
    playRect_     = MakeRect(kMenuLeftMargin, kPlayY,     playText_.Width(),     playText_.Height());
    dontPlayRect_ = MakeRect(kMenuLeftMargin, kDontPlayY, dontPlayText_.Width(), dontPlayText_.Height());
}

void Game::LoadGameResources() {
    SDL_Renderer* renderer = graphics_.GetRenderer();
    const float pixelScale = graphics_.GetPixelScale();

    roomBackground_.LoadSvgFitted(renderer, AssetPath("room.svg"),
                                  kWindowWidth, kWindowHeight, pixelScale);
    cursorSprite_.LoadSvg(renderer, AssetPath("cursor.svg"), 1.0f, pixelScale);

    Font clockFont;
    OpenUiFont(clockFont, kClockFontSize, pixelScale);
    clockText_.CreateFromText(renderer, clockFont.Get(), "12 AM", kBlack, pixelScale);

    LoadEnemies();
}

void Game::LoadEnemies() {
    SDL_Renderer* renderer = graphics_.GetRenderer();
    const float pixelScale = graphics_.GetPixelScale();

    Enemy& egg = enemies_[ENEMY_EGG];
    egg.alive.LoadSvg(renderer, AssetPath("egg.svg"), kEnemyScale, pixelScale);
    egg.shotSprite.LoadSvg(renderer, AssetPath("egg_shot.svg"), kEnemyScale, pixelScale);
    egg.spawnX = 60;
    egg.spawnY = 250;
    egg.shotOffsetX = 57;
    egg.shotOffsetY = 100;

    Enemy& buddy = enemies_[ENEMY_BUDDY];
    buddy.alive.LoadSvg(renderer, AssetPath("buddy.svg"), kEnemyScale, pixelScale);
    buddy.shotSprite.LoadSvg(renderer, AssetPath("buddy_shot.svg"), kEnemyScale, pixelScale);
    buddy.spawnX = 650;
    buddy.spawnY = 270;
    buddy.shotOffsetX = -198;
    buddy.shotOffsetY = 210;

    Enemy& clown = enemies_[ENEMY_CLOWN];
    clown.alive.LoadSvg(renderer, AssetPath("clown.svg"), kEnemyScale, pixelScale);
    clown.overlay.LoadSvg(renderer, AssetPath("clown_blood.svg"), kEnemyScale, pixelScale);
    clown.spawnX = (kWindowWidth - clown.alive.Width()) / 2;
    clown.spawnY = static_cast<int>((kWindowHeight - clown.alive.Height()) / 2.5);
    clown.shotOffsetX = -25;
    clown.shotOffsetY = 70;
    clown.shotAngle = 192.0;

    for (int i = 0; i < ENEMY_COUNT; ++i) {
        enemies_[i].Reset();
    }
}

void Game::LoadIntroResources() {
    Font introFont;
    OpenUiFont(introFont, kIntroFontSize, graphics_.GetPixelScale());
    introText_.CreateFromText(graphics_.GetRenderer(), introFont.Get(),
                              kIntroText, kWhite, graphics_.GetPixelScale());
}

void Game::LoadEndingResources() {
    SDL_Renderer* renderer = graphics_.GetRenderer();
    const float pixelScale = graphics_.GetPixelScale();

    Font endingFont;
    OpenUiFont(endingFont, kEndingFontSize, pixelScale);

    for (int i = 0; i < kHourCount; ++i) {
        hourTextures_[i].CreateFromText(renderer, endingFont.Get(), HourLabel(i + 1), kWhite, pixelScale);
    }
    finalText_.CreateFromText(renderer, endingFont.Get(), HourLabel(kHourCount), kGold, pixelScale);
}

// ---------------------------------------------------------------------------
// State transitions
// ---------------------------------------------------------------------------

void Game::StartIntro(Uint32 now) {
    for (int i = 0; i < ENEMY_COUNT; ++i) {
        enemies_[i].Reset();
    }
    introStartTime_ = now;
    state_ = STATE_INTRO;

    SetHandCursor(false);
    SDL_ShowCursor(SDL_DISABLE);
}

void Game::StartGame(Uint32 now) {
    for (int i = 0; i < ENEMY_COUNT; ++i) {
        enemies_[i].Reset();
    }
    gameStartTime_ = now;
    state_ = STATE_PLAYING;

    SetHandCursor(false);
    SDL_ShowCursor(SDL_DISABLE);
}

void Game::StartEnding(Uint32 now) {
    state_ = STATE_ENDING;
    endingStartTime_ = now;
    confetti_.clear();
    confettiSpawned_ = false;
}

void Game::ReturnToMenu() {
    state_ = STATE_MENU;
    confetti_.clear();
    confettiSpawned_ = false;
    SDL_ShowCursor(SDL_ENABLE);
}

// ---------------------------------------------------------------------------
// Input
// ---------------------------------------------------------------------------

void Game::ProcessEvents(Uint32 now) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running_ = false;
            continue;
        }

        if (event.type != SDL_MOUSEBUTTONDOWN || event.button.button != SDL_BUTTON_LEFT) {
            continue;
        }

        switch (state_) {
        case STATE_MENU:
            OnMenuClick(event.button.x, event.button.y, now);
            break;
        case STATE_PLAYING:
            OnPlayClick(event.button.x, event.button.y, now);
            break;
        default:
            break;
        }
    }
}

void Game::OnMenuClick(int x, int y, Uint32 now) {
    if (PointInRect(x, y, playRect_)) {
        StartIntro(now);
    }
}

void Game::OnPlayClick(int x, int y, Uint32 now) {
    for (int i = ENEMY_COUNT - 1; i >= 0; --i) {
        if (enemies_[i].CanBeHit(x, y)) {
            enemies_[i].MarkShot(now);
            break;
        }
    }
}

void Game::SetHandCursor(bool active) {
    if (active == handCursorActive_) return;

    SDL_Cursor* cursor = active ? handCursor_ : arrowCursor_;
    if (cursor) {
        SDL_SetCursor(cursor);
    }
    handCursorActive_ = active;
}

// ---------------------------------------------------------------------------
// Update
// ---------------------------------------------------------------------------

void Game::Update(Uint32 now, float dt) {
    switch (state_) {
    case STATE_MENU:    UpdateMenu();            break;
    case STATE_INTRO:   UpdateIntro(now);        break;
    case STATE_PLAYING: UpdatePlaying(now);      break;
    case STATE_ENDING:  UpdateEnding(now, dt);   break;
    }
}

void Game::UpdateMenu() {
    int mouseX = 0;
    int mouseY = 0;
    SDL_GetMouseState(&mouseX, &mouseY);
    SetHandCursor(PointInRect(mouseX, mouseY, playRect_));
}

void Game::UpdateIntro(Uint32 now) {
    if (now - introStartTime_ >= kIntroHoldMs + kIntroFadeMs) {
        StartGame(now);
    }
}

void Game::UpdatePlaying(Uint32 now) {
    bool allShot = true;

    for (int i = 0; i < ENEMY_COUNT; ++i) {
        Enemy& enemy = enemies_[i];

        if (!enemy.isVisible) {
            const bool isFirst = (i == 0);
            const bool previousDone = isFirst || enemies_[i - 1].isShot;
            const Uint32 since = isFirst ? gameStartTime_ : enemies_[i - 1].shotTime;

            if (previousDone && now - since >= kEnemySpawnDelayMs) {
                enemy.isVisible = true;
            }
        }

        if (!enemy.isShot) {
            allShot = false;
        }
    }

    if (allShot) {
        StartEnding(now);
    }
}

void Game::UpdateEnding(Uint32 now, float dt) {
    const Uint32 elapsed = now - endingStartTime_;

    if (elapsed >= kClockPhaseEndMs) {
        if (!confettiSpawned_) {
            SpawnConfetti();
        }
        UpdateConfetti(dt);
    }

    if (elapsed >= kEndingDurationMs) {
        ReturnToMenu();
    }
}

void Game::SpawnConfetti() {
    confettiSpawned_ = true;
    confetti_.clear();
    confetti_.reserve(kConfettiCount);

    for (int i = 0; i < kConfettiCount; ++i) {
        Confetti p;
        p.x     = static_cast<float>(RandomRange(0, kWindowWidth - 1));
        p.y     = static_cast<float>(-RandomRange(0, kWindowHeight - 1));
        p.vx    = RandomRange(-100, 99) / 50.0f * kLegacyFps;
        p.vy    = (2.0f + RandomRange(0, 399) / 100.0f) * kLegacyFps;
        p.color = kConfettiPalette[RandomRange(0, kConfettiPaletteSize - 1)];
        p.size  = RandomRange(kConfettiMinSize, kConfettiMaxSize);
        confetti_.push_back(p);
    }
}

void Game::UpdateConfetti(float dt) {
    for (std::size_t i = 0; i < confetti_.size(); ++i) {
        Confetti& p = confetti_[i];
        p.x += p.vx * dt;
        p.y += p.vy * dt;

        if (p.y > kWindowHeight) {
            p.y = -10.0f;
            p.x = static_cast<float>(RandomRange(0, kWindowWidth - 1));
        }
    }
}

// ---------------------------------------------------------------------------
// Rendering
// ---------------------------------------------------------------------------

void Game::Render(Uint32 now) const {
    SDL_Renderer* renderer = graphics_.GetRenderer();

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    if (state_ == STATE_MENU) {
        RenderMenu();
    } else {
        RenderScene();
        if (state_ == STATE_PLAYING) {
            RenderHud();
        } else if (state_ == STATE_INTRO) {
            RenderHud();
            RenderIntro(now);
        } else {
            RenderEnding(now);
        }
    }

    SDL_RenderPresent(renderer);
}

void Game::RenderMenu() const {
    SDL_Renderer* renderer = graphics_.GetRenderer();

    menuBackground_.Draw(renderer, ScreenRect());
    titleText_.Draw(renderer, titleRect_);
    playText_.Draw(renderer, playRect_);
    dontPlayText_.Draw(renderer, dontPlayRect_);
}

void Game::RenderScene() const {
    SDL_Renderer* renderer = graphics_.GetRenderer();

    roomBackground_.Draw(renderer, ScreenRect());
    for (int i = 0; i < ENEMY_COUNT; ++i) {
        enemies_[i].Draw(renderer);
    }
}

void Game::RenderHud() const {
    SDL_Renderer* renderer = graphics_.GetRenderer();

    clockText_.DrawAt(renderer,
                      kWindowWidth - clockText_.Width() - kClockMargin,
                      kClockMargin);

    if (cursorSprite_.IsValid()) {
        int mouseX = 0;
        int mouseY = 0;
        SDL_GetMouseState(&mouseX, &mouseY);
        cursorSprite_.DrawAt(renderer, mouseX, mouseY);
    }
}

void Game::RenderIntro(Uint32 now) const {
    SDL_Renderer* renderer = graphics_.GetRenderer();
    const Uint32 elapsed = now - introStartTime_;

    // Fully opaque during the hold, then fades smoothly to transparent.
    int alpha = 255;
    if (elapsed >= kIntroHoldMs) {
        const float t = static_cast<float>(elapsed - kIntroHoldMs) / kIntroFadeMs;
        alpha = (t >= 1.0f) ? 0 : static_cast<int>(255.0f * (1.0f - t));
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, static_cast<Uint8>(alpha));
    const SDL_Rect screen = ScreenRect();
    SDL_RenderFillRect(renderer, &screen);

    vhs_.Draw(renderer, introText_,
              (kWindowWidth  - introText_.Width())  / 2,
              (kWindowHeight - introText_.Height()) / 2,
              alpha, elapsed);
}

void Game::RenderEnding(Uint32 now) const {
    SDL_Renderer* renderer = graphics_.GetRenderer();
    const Uint32 elapsed = now - endingStartTime_;

    int alpha = 255;
    if (elapsed < kFadeDurationMs) {
        alpha = static_cast<int>(elapsed * 255.0f / kFadeDurationMs);
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, static_cast<Uint8>(alpha));
    const SDL_Rect screen = ScreenRect();
    SDL_RenderFillRect(renderer, &screen);

    if (elapsed < kFadeDurationMs) {
        return;
    }

    if (elapsed < kClockPhaseEndMs) {
        const float progress =
            static_cast<float>(elapsed - kFadeDurationMs) / (kClockPhaseEndMs - kFadeDurationMs);
        int index = static_cast<int>(progress * kHourCount);
        if (index > kHourCount - 1) index = kHourCount - 1;

        DrawCentered(hourTextures_[index], 0);
        return;
    }

    DrawCentered(finalText_, kFinalLabelOffsetY);

    for (std::size_t i = 0; i < confetti_.size(); ++i) {
        const Confetti& p = confetti_[i];
        SDL_SetRenderDrawColor(renderer, p.color.r, p.color.g, p.color.b, 255);
        const SDL_Rect r = MakeRect(static_cast<int>(p.x), static_cast<int>(p.y), p.size, p.size);
        SDL_RenderFillRect(renderer, &r);
    }
}

void Game::DrawCentered(const Texture& texture, int offsetY) const {
    texture.DrawAt(graphics_.GetRenderer(),
                   (kWindowWidth  - texture.Width())  / 2,
                   (kWindowHeight - texture.Height()) / 2 + offsetY);
}

// ---------------------------------------------------------------------------
// Main loop
// ---------------------------------------------------------------------------

void Game::Run() {
    Uint32 previous = SDL_GetTicks();

    while (running_) {
        const Uint32 now = SDL_GetTicks();
        float dt = (now - previous) / 1000.0f;
        if (dt > kMaxFrameSeconds) {
            dt = kMaxFrameSeconds;
        }
        previous = now;

        ProcessEvents(now);
        Update(now, dt);
        Render(now);

        if (!graphics_.HasVsync()) {
            SDL_Delay(1);
        }
    }
}