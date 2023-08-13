#include <SDL2/SDL.h>
#include <memory>

#include "Game.h"


int main ( )
{

    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    std::unique_ptr<Game> game ( new Game () );

    bool loadTheMap = game -> startMenu();

    if ( game -> running () )
    {
        game -> init ( "PAC-MAN", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 700, 700, false, loadTheMap );

        game -> handleEvents();
        game -> render();
        game -> update();

        SDL_Delay ( 1500 );
    }

    while ( game -> running() )
    {
        frameStart = SDL_GetTicks();

        game -> handleEvents();
        game -> render();
        game -> update();

        frameTime = SDL_GetTicks() - frameStart;

        if ( frameDelay > frameTime )
        {
            SDL_Delay ( frameDelay - frameTime );
        }
    }

    game -> clean();

    return 0;
}
