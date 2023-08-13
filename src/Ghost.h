#ifndef PAC_MAN_GHOST_H
#define PAC_MAN_GHOST_H

#include <deque>
#include <memory>
#include <SDL2/SDL.h>

#include "TextureManager.h"
#include "Map.h"


class Ghost
{
public:
    Ghost  ( std::shared_ptr<Map> map );
    ~Ghost () = default;

    virtual void update ( std::pair<int,int> pacmanCoordinates, int directionOfPacman ) = 0;
    virtual void chase  ( std::pair<int,int> pacmanCoordinates, int directionOfPacman ) = 0;
    virtual void changeChaseMode () = 0;
    virtual void render   () const  = 0;
    virtual void newRound ()        = 0;
    virtual void wasEaten ()        = 0;
    void setBlinking ( bool type );
    void frighten ();

    virtual bool getChaseMode () const = 0;
    virtual std::pair<int,int> getCoordinates () const = 0;

protected:

    const char * FRIGHTEN_GHOST         = "assets/frigtenGhost.png";
    const char * FRIGHTEN_WHITE_GHOST   = "assets/frigtenWhiteGhost.png";

    std::deque < std::pair<int,int> > nextMove ( std::pair < int, int > dest );
    int  randomDirection ();
    bool crossRoads     ();
    void randomPath     ( int & direction );

    SDL_Texture * frigtenGhost;
    SDL_Texture * frigtenWhiteGhost;


    SDL_Rect m_Src;
    SDL_Rect m_Dest;

    int m_Speed;

    std::shared_ptr<Map> m_Map;

    int m_Direction;
    int countOfMoves;

    SDL_Texture * m_CurTexture;
    bool go;
    bool blinking;

};


#endif //PAC_MAN_GHOST_H
