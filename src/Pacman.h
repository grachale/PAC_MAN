#ifndef PAC_MAN_PACMAN_H
#define PAC_MAN_PACMAN_H

#include <SDL2/SDL.h>
#include <memory>
#include <vector>

#include "TextureManager.h"
#include "Map.h"



class Pacman
{
public:
    Pacman  ( std::shared_ptr<Map> curMap, int x, int y );
    ~Pacman () = default;
    void newRound ();
    void render ();
    void update ( bool & pacmanChases, int & score, int & dotsTaken, int & time );

    int getDirection () const;
    std::pair<int,int> getCoordinates () const;

private:

    SDL_Texture * textureLeftHalf;
    SDL_Texture * textureRigthHalf;
    SDL_Texture * textureUpHalf;
    SDL_Texture * textureDownHalf;

    SDL_Texture * textureLeft;
    SDL_Texture * textureRigth;
    SDL_Texture * textureUp;
    SDL_Texture * textureDown;

    SDL_Texture  * textureFull;
    SDL_Texture  * previous;


    bool checkLeft  ( int up, int mid, int down ) const;
    bool checkRight ( int up, int mid, int down ) const;
    bool checkUp    ( int up, int mid, int down ) const;
    bool checkDown  ( int up, int mid, int down ) const;

    std::vector<SDL_Texture *> m_TextureLeft;
    std::vector<SDL_Texture *> m_TextureRight;
    std::vector<SDL_Texture *> m_TextureUp;
    std::vector<SDL_Texture *> m_TextureDown;

    unsigned int frameOfPacman;

    std::shared_ptr<Map> m_CurMap;


    int m_Speed     = 1;
    int m_Direction = 0;

    SDL_Rect m_Src;
    SDL_Rect m_Dest;

    int START_POSITION_X;
    int START_POSITION_Y;


};


#endif //PAC_MAN_PACMAN_H
