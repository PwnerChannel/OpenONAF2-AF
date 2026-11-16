// Game.h - main game class: states, input, update and rendering.
#ifndef GAME_H
#define GAME_H

#include <SDL.h>

#include <vector>

#include "Config.h"
#include "Confetti.h"
#include "Enemy.h"
#include "Graphics.h"
#include "SdlRuntime.h"
#include "Texture.h"
#include "VhsEffect.h"

class Game {
public:
    Game();
    ~Game();

    void Run();

private:
    enum State {
        STATE_MENU,
        STATE_INTRO,
        STATE_PLAYING,
        STATE_ENDING
    };

    Game(const Game&);
    Game& operator=(const Game&);

    // Resource loading
    void LoadResources();
    void LoadMenuResources();
    void LoadGameResources();
    void LoadEnemies();
    void LoadIntroResources();
    void LoadEndingResources();

    // State transitions
    void StartIntro(Uint32 now);
    void StartGame(Uint32 now);
    void StartEnding(Uint32 now);
    void ReturnToMenu();

    // Input
    void ProcessEvents(Uint32 now);
    void OnMenuClick(int x, int y, Uint32 now);
    void OnPlayClick(int x, int y, Uint32 now);
    void SetHandCursor(bool active);

    // Update
    void Update(Uint32 now, float dt);
    void UpdateMenu();
    void UpdateIntro(Uint32 now);
    void UpdatePlaying(Uint32 now);
    void UpdateEnding(Uint32 now, float dt);
    void SpawnConfetti();
    void UpdateConfetti(float dt);

    // Rendering
    void Render(Uint32 now) const;
    void RenderMenu() const;
    void RenderScene() const;
    void RenderHud() const;
    void RenderIntro(Uint32 now) const;
    void RenderEnding(Uint32 now) const;
    void DrawCentered(const Texture& texture, int offsetY) const;

    SdlRuntime runtime_;
    Graphics   graphics_;

    SDL_Cursor* arrowCursor_;
    SDL_Cursor* handCursor_;
    bool        handCursorActive_;

    // Menu
    Texture  menuBackground_;
    Texture  titleText_;
    Texture  playText_;
    Texture  dontPlayText_;
    SDL_Rect titleRect_;
    SDL_Rect playRect_;
    SDL_Rect dontPlayRect_;

    // Gameplay
    Texture roomBackground_;
    Texture cursorSprite_;
    Texture clockText_;
    Enemy   enemies_[ENEMY_COUNT];

    // Intro
    Texture   introText_;
    VhsEffect vhs_;

    // Ending
    Texture               hourTextures_[kHourCount];
    Texture               finalText_;
    std::vector<Confetti> confetti_;
    bool                  confettiSpawned_;

    State  state_;
    bool   running_;
    Uint32 introStartTime_;
    Uint32 gameStartTime_;
    Uint32 endingStartTime_;
};

#endif // GAME_H