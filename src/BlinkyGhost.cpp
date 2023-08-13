#include "BlinkyGhost.h"

using namespace std;

BlinkyGhost::BlinkyGhost ( shared_ptr<Map> map, int chosenDifficultyLevel, int x, int y )
    : Ghost ( map )
    {
        // uploading the image of Blinky ghost
        m_CurTexture = TextureManager::LoadTexture(GHOST_IMAGE_BLINKY_UP);
        m_ChosenDifficultyLevel = chosenDifficultyLevel;
        START_POSITION_Y = y;
        START_POSITION_X = x;

        // position on map
        m_Dest . x = START_POSITION_X;
        m_Dest . y = START_POSITION_Y;
        // full size of ghost
        m_Dest . w = m_Dest . h = 30;
        chaseMode = false;

        // Blinky directly follows PACMAN
        m_Trace = nextMove ( make_pair ( 29, 43) );

        go = true;

        if ( m_ChosenDifficultyLevel == 1 )
            COUNT_OF_MOVES_AFTER_BFS = 25;
        else
            COUNT_OF_MOVES_AFTER_BFS = 13;

    }

/**
 * Update position of the ghost on map according to level of difficulty and PACMAN location.
 *
 * @param pacmanCoordinates coordinates of PACMAN
 * @param directionOfPacman direction of PACMAN
 *
*/
void BlinkyGhost::update(std::pair<int, int> pacmanCoordinates, int directionOfPacman )
{
    if ( m_ChosenDifficultyLevel != 0 ) {
        if (countOfMoves >= COUNT_OF_MOVES_AFTER_BFS || m_Trace . empty() ) {
            m_Trace = nextMove(make_pair(pacmanCoordinates.second, pacmanCoordinates.first));
            countOfMoves = 0;
        } else
            SDL_Delay(10);
        // go is a variable to slow down the ghost (2 pacman moves = 1 ghost move)
        if (go) {
            m_Dest.y = m_Trace.front().first;
            m_Dest.x = m_Trace.front().second;
            m_Trace.pop_front();
            countOfMoves++;
            // speeding up on the last level (1 pacman move = 1 ghost move)
            if ( m_ChosenDifficultyLevel != 2 )
                go = false;
        } else {
            go = true;
        }

    } else {
        randomPath( m_Direction );
        SDL_Delay(30);
    }
}

/**
 * Render the ghost.
 *
*/
void BlinkyGhost::render () const
{
    SDL_Rect temp = m_Dest;

    temp . x = (temp . x) * 10 + 70;
    temp . y = (temp . y) * 10 + 70;

    TextureManager::Draw ( m_CurTexture, m_Src , temp);
}

/**
 * Setting everything up for the new round.
 *
*/
void BlinkyGhost::newRound()
{
    // position of 2d array
    m_Dest . x = START_POSITION_X;
    m_Dest . y = START_POSITION_Y;

    // Blinky directly follows PACMAN ( (29,43) - coordinates of PACMAN )
    m_Trace. clear ();

    go = true;
}

/**
 * Setting everything up the ghost after being eaten by pacman.
 *
 *
*/
void BlinkyGhost::wasEaten()
{
    m_Dest . x  = START_POSITION_X;
    m_Dest . y  = START_POSITION_Y;
    m_CurTexture = TextureManager::LoadTexture(GHOST_IMAGE_BLINKY_UP );
    countOfMoves = COUNT_OF_MOVES_AFTER_BFS;
    go = false;
}

/**
 * Getting coordinates of the Ghost.
 *
 * @return coordinates of ghost
*/
std::pair<int, int> BlinkyGhost::getCoordinates() const
{
    return make_pair ( m_Dest . x, m_Dest . y );
}

/**
 * Changing chase mode.
 *
*/
void BlinkyGhost::changeChaseMode()
{
    chaseMode = !chaseMode;
}

/**
 * Getting chase mode.
 *
 * @return get chase mode
*/
bool BlinkyGhost::getChaseMode() const
{
    return chaseMode;
}

/**
 * Setting everything up for chase mode after being eaten by pacman.
 *
 * @param directionOfPacman direction of pacman
 * @param pacmanCoordinates coordinates of pacman
 *
*/
void BlinkyGhost::chase(std::pair<int, int> pacmanCoordinates, int directionOfPacman)
{
    m_CurTexture = TextureManager::LoadTexture(GHOST_IMAGE_BLINKY_UP );
    countOfMoves = COUNT_OF_MOVES_AFTER_BFS;
    update( pacmanCoordinates, directionOfPacman );
    go = false;
}