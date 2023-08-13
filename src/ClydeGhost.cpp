#include "ClydeGhost.h"

#include <math.h>

using namespace std;

ClydeGhost::ClydeGhost ( shared_ptr<Map> map, int chosenDifficultyLevel, int x, int y )
        : Ghost ( map )
{
    // uploading the image of Pinky ghost
    m_CurTexture = TextureManager::LoadTexture(GHOST_IMAGE_CLYDE_UP);
    START_POSITION_X = x;
    START_POSITION_Y = y;
    // position on the map
    m_Dest . x = START_POSITION_X;
    m_Dest . y = START_POSITION_Y;
    // full size of ghost
    m_Dest . w = m_Dest . h = 30;

    chaseMode = false;
    go = true;
    m_Direction = 2;
    m_ChosenDifficultyLevel = chosenDifficultyLevel;

    if ( m_ChosenDifficultyLevel == 1 )
    {
        COUNT_OF_MOVES_AFTER_BFS = 22;
        safeLength = 15;
    } else {
        COUNT_OF_MOVES_AFTER_BFS = 15;
        safeLength = 8;
    }

    // Pinky chases towards the spot 2 Pac-Dots
    for ( int i = 0; i < COUNT_OF_MOVES_AFTER_BFS; ++i )
    {
        m_Trace . push_back ( make_pair ( START_POSITION_Y, START_POSITION_X ) );
    }
}



/**
 * Update position of the ghost on map according to level of difficulty and PACMAN location.
 *
 * @param pacmanCoordinates coordinates of PACMAN
 * @param directionOfPacman direction of PACMAN
 *
*/
void ClydeGhost::update(std::pair<int, int> pacmanCoordinates, int directionOfPacman)
{
        // if clyde is further than safe distance between PACMAN and himself, he chases directly PACMAN, otherwise he goes randomly
        // depends on level of difficulty
        if ( distanceBetweenObjects ( pacmanCoordinates, this -> getCoordinates() ) > safeLength &&
             m_ChosenDifficultyLevel != 0 ) {

            if ( countOfMoves >= COUNT_OF_MOVES_AFTER_BFS || m_Trace . empty () ) {
                m_Trace = nextMove(make_pair(pacmanCoordinates.second, pacmanCoordinates.first));
                countOfMoves = 0;
            } else
                SDL_Delay(10);
            // go is a variable to slow down the ghost (2 pacman moves = 1 ghost move)
            if (go) {
                m_Dest.y = m_Trace.front() . first;
                m_Dest.x = m_Trace.front() . second;
                m_Trace.pop_front();
                countOfMoves++;
                // speeding up on the last level (1 pacman move = 1 ghost move)
                if ( m_ChosenDifficultyLevel != 2 )
                    go = false;
            } else {
                go = true;
            }
        } else {
            m_Trace.clear();
            randomPath( m_Direction );
            SDL_Delay(30);
        }
}


/**
 * Render the ghost.
 *
*/
void ClydeGhost::render () const
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
void ClydeGhost::newRound()
{
    m_Dest . x = START_POSITION_X;
    m_Dest . y = START_POSITION_Y;

    go = false;

    m_Direction = 2;

    m_Trace . clear ();
}

/**
 * Setting everything up the ghost after being eaten by pacman.
 *
 *
*/
void ClydeGhost::wasEaten ()
{
    m_Dest . x  = START_POSITION_X;
    m_Dest . y  = START_POSITION_Y;
    m_CurTexture = TextureManager::LoadTexture(GHOST_IMAGE_CLYDE_UP );
    countOfMoves = COUNT_OF_MOVES_AFTER_BFS;
    go = false;
}

/**
 * Getting coordinates of the Ghost.
 *
 * @return coordinates of ghost
*/
std::pair<int, int> ClydeGhost::getCoordinates() const
{
    return make_pair ( m_Dest . x, m_Dest . y );
}

/**
 * Finding the distance between objects with pythagor theorem.
 *
 * @param object1 coordinates of the first object
 * @param object2 coordinates of the second object
 *
 * @return distance between them
*/
int ClydeGhost::distanceBetweenObjects(pair<int, int> object1, pair<int, int> object2) const
{
    return sqrt ( pow ( object1 . first - object1 . first, 2) + pow(object1 . second - object2 . second, 2) );
}

/**
 * Changing chase mode.
 *
*/
void ClydeGhost::changeChaseMode()
{
    chaseMode = !chaseMode;
}

/**
 * Getting chase mode.
 *
 * @return get chase mode
*/
bool ClydeGhost::getChaseMode() const
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
void ClydeGhost::chase(std::pair<int, int> pacmanCoordinates, int directionOfPacman)
{
    m_CurTexture = TextureManager::LoadTexture(GHOST_IMAGE_CLYDE_UP );
    countOfMoves = COUNT_OF_MOVES_AFTER_BFS;
    update( pacmanCoordinates, directionOfPacman );
    go = false;
}