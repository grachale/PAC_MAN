#ifndef PAC_MAN_GAME_H
#define PAC_MAN_GAME_H

#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <memory>
#include <vector>


class Game
{
public:
     Game();
     ~Game() = default;

     void init ( const char * title, int xpos, int ypos, int width, int height, bool fullScreen, bool loadTheMap );
     void handleEvents ();
     void update       ();
     void render       ();
     void clean        ();
     bool startMenu    ();

     bool doObjectsIntersect ( std::pair<int,int> coordinates1, std::pair<int,int> coordinates2 ) const;
     bool running            () const;
     void newLevel           ();

     static SDL_Renderer * renderer;
     static SDL_Event      event;

private:
    SDL_Window * window;
    // time of the game
    int m_Time;
    int m_Score;
    int m_Lives;
    int m_DotsEaten;
    int m_ChosenDifficultyLevel;

    bool m_IsRunning;
    bool pacmanChases;

    const std::string happyWords     [6] = { "WOW", "INSANE", "YOU'ARE CRAZY", "OMG", "IMPRESSIVE", "GOOD JOB" };
    const std::string difficultyLevel[3] = { "NOVICE", "STANDARD", "EXPERT" };

    int PACMAN_POSITION_X = 29;
    int PACMAN_POSITION_Y = 43;

    int RED_GHOST_POSITION_X = 24;
    int RED_GHOST_POSITION_Y = 21;

    int ORANGE_GHOST_POSITION_X = 20;
    int ORANGE_GHOST_POSITION_Y = 21;

    int PINKY_GHOST_POSITION_X = 28;
    int PINKY_GHOST_POSITION_Y = 21;
};

#endif //PAC_MAN_GAME_H
