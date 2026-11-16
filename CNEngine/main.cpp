// main.cpp - program entry point.
#include <SDL.h>   // needed so SDL's main() redefinition works on Windows

#include <cstdlib>
#include <ctime>
#include <exception>
#include <iostream>

#include "Config.h"
#include "Game.h"

int main(int, char*[]) {
    try {
        std::srand(static_cast<unsigned int>(std::time(NULL)));

        Game game;
        game.Run();
    } catch (const std::exception& error) {
        std::cerr << "Fatal error: " << error.what() << std::endl;
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, kWindowTitle, error.what(), NULL);
        return 1;
    }
    return 0;
}