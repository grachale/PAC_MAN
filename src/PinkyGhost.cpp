#include "PinkyGhost.h"

using namespace std;


PinkyGhost::PinkyGhost ( shared_ptr<Map> map, int chosenDifficultyLevel, int x, int y )
        : Ghost ( map )
{
    // uploading the image of Pinky ghost
    m_CurTexture = TextureManager::LoadTexture(GHOST_IMAGE_PINKY_UP);

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
        COUNT_OF_MOVES_AFTER_BFS = 27;
    else
        COUNT_OF_MOVES_AFTER_BFS = 10;

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
void PinkyGhost::update( std::pair<int, int> pacmanCoordinates, int directionOfPacman ) {
    if ( m_ChosenDifficultyLevel != 0 ) {
        if (countOfMoves >= COUNT_OF_MOVES_AFTER_BFS || m_Trace.empty()) {
            m_Trace = nextMove(twoCellsInFrontOfPacman(pacmanCoordinates, directionOfPacman));
            countOfMoves = 0;
        } else
            SDL_Delay ( 10 );
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
 * Getting coordinates of the Ghost.
 *
 * @return coordinates of ghost
*/
std::pair<int, int> PinkyGhost::getCoordinates() const
{
    return make_pair ( m_Dest . x, m_Dest . y );
}


/**
 * Finding the coordinates of the cell in front of PACMAN which is located
 * on distance 2. (Original PACMAN rule)
 *
 * @param pacmanCoordinates pacman coordinates
 * @param directionOfPacman direction of the pacman
 *
 * @return coordinates of the cell
*/
std::pair<int, int> PinkyGhost::twoCellsInFrontOfPacman ( std::pair<int, int> pacmanCoordinates, int directionOfPacman ) const
{
    int x = pacmanCoordinates . first;
    int y = pacmanCoordinates . second;

    switch ( directionOfPacman )
    {
        // check the right
        case 0:
            if (   55 >= x + 6 &&
                   (m_Map -> m_Map [y + 1][x + 3] == 0 || m_Map -> m_Map [y + 1][x + 3] == 2 ) &&
                   m_Map -> m_Map [y][x + 3] == 0 && m_Map -> m_Map [y + 2][x + 3] == 0 &&
                   (m_Map -> m_Map [y + 1][x + 4] == 0 || m_Map -> m_Map [y + 1][x + 4] == 2 ) &&
                   m_Map -> m_Map [y][x + 4] == 0 && m_Map -> m_Map [y + 2][x + 4] == 0 &&
                   (m_Map -> m_Map [y + 1][x + 5] == 0 || m_Map -> m_Map [y + 1][x + 5] == 2 ) &&
                   m_Map -> m_Map [y][x + 5] == 0 && m_Map -> m_Map [y + 2][x + 5] == 0 &&
                   (m_Map -> m_Map [y + 1][x + 6] == 0 || m_Map -> m_Map [y + 1][x + 6] == 2 ) &&
                   m_Map -> m_Map [y][x + 6] == 0 && m_Map -> m_Map [y + 2][x + 6] == 0 )
                return make_pair ( y, x + 4 );
            break;
        // check down
        case 1:
            if (    58 >= y + 6 &&
                    (m_Map -> m_Map [y + 3][x + 1] == 0 || m_Map -> m_Map [y + 3][x + 1] == 2 ) &&
                    m_Map -> m_Map [y + 3][x] == 0 && m_Map -> m_Map [y + 3][x + 2] == 0 &&
                    (m_Map -> m_Map [y + 4][x + 1] == 0 || m_Map -> m_Map [y + 4][x + 1] == 2 ) &&
                    m_Map -> m_Map [y + 4][x] == 0 && m_Map -> m_Map [y + 4][x + 2] == 0 &&
                    (m_Map -> m_Map [y + 5][x + 1] == 0 || m_Map -> m_Map [y + 5][x + 1] == 2 ) &&
                    m_Map -> m_Map [y + 5][x] == 0 && m_Map -> m_Map [y + 5][x + 2] == 0 &&
                    (m_Map -> m_Map [y + 6][x + 1] == 0 || m_Map -> m_Map [y + 6][x + 1] == 2 ) &&
                    m_Map -> m_Map [y + 6][x] == 0 && m_Map -> m_Map [y + 6][x + 2] == 0
                      )
                return make_pair ( y + 4, x );
            break;
            // check the left
        case 2:
            if (   55 >= x - 4 && x - 4 >= 0 &&
                   (m_Map -> m_Map [y + 1][x - 1] == 0 || m_Map -> m_Map[y + 1][x - 1] == 2 ) &&
                   m_Map -> m_Map [y][x - 1] == 0 && m_Map -> m_Map [y + 2][x - 1] == 0 &&
                   (m_Map -> m_Map [y + 1][x - 2] == 0 || m_Map -> m_Map [y + 1][x - 2] == 2 ) &&
                   m_Map -> m_Map [y][x - 2] == 0 && m_Map -> m_Map [y + 2][x - 2] == 0 &&
                   (m_Map -> m_Map [y + 1][x - 3] == 0 || m_Map -> m_Map [y + 1][x - 3] == 2 ) &&
                   m_Map -> m_Map [y][x - 3] == 0 && m_Map -> m_Map [y + 2][x - 3] == 0 &&
                   (m_Map -> m_Map [y + 1][x - 4] == 0 || m_Map -> m_Map [y + 1][x - 4] == 2 ) &&
                   m_Map -> m_Map [y][x - 4] == 0 && m_Map -> m_Map [y + 2][x - 4] == 0
                    )
                return make_pair ( y, x - 2 );
            break;
            // check up
        case 3:
            if (    58 >= y - 4 && y - 4 >= 0 &&
                    (m_Map -> m_Map [y - 1][x + 1] == 0 || m_Map -> m_Map [y - 1][x + 1] == 2 ) &&
                    m_Map -> m_Map [y - 1][x] == 0 && m_Map -> m_Map [y - 1][x + 2] == 0 &&
                    (m_Map -> m_Map [y - 2][x + 1] == 0 || m_Map -> m_Map [y - 2][x + 1] == 2 ) &&
                    m_Map -> m_Map [y - 2][x] == 0 && m_Map -> m_Map [y - 2][x + 2] == 0 &&
                    (m_Map -> m_Map [y - 3][x + 1] == 0 || m_Map -> m_Map [y - 3][x + 1] == 2 ) &&
                    m_Map -> m_Map [y - 3][x] == 0 && m_Map -> m_Map [y - 3][x + 2] == 0 &&
                    (m_Map -> m_Map [y - 4][x + 1] == 0 || m_Map -> m_Map [y - 4][x + 1] == 2 ) &&
                    m_Map -> m_Map [y - 4][x] == 0 && m_Map -> m_Map [y - 4][x + 2] == 0


                    )
                       return make_pair ( y - 2, x );
        default:
            return make_pair ( y, x );
    }
        return make_pair ( y, x );
}

/**
 * Render the ghost.
 *
*/
void PinkyGhost::render () const
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
void PinkyGhost::newRound()
{
    m_Dest . x = START_POSITION_X;
    m_Dest . y = START_POSITION_Y;
    go = true;

    // Pinky chases towards the spot 2 Pac-Dots ( (29,43) - coordinates of PACMAN )
    m_Trace . clear ();
}

/**
 * Setting everything up the ghost after being eaten by pacman.
 *
 *
*/
void PinkyGhost::wasEaten()
{
    m_Dest . x  = START_POSITION_X;
    m_Dest . y  = START_POSITION_Y;

    m_CurTexture = TextureManager::LoadTexture(GHOST_IMAGE_PINKY_UP );
    countOfMoves = COUNT_OF_MOVES_AFTER_BFS;
    go = false;
}

/**
 * Changing chase mode.
 *
*/
void PinkyGhost::changeChaseMode()
{
    chaseMode = !chaseMode;
}

/**
 * Getting chase mode.
 *
 * @return get chase mode
*/
bool PinkyGhost::getChaseMode() const
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
void PinkyGhost::chase(std::pair<int, int> pacmanCoordinates, int directionOfPacman)
{
    m_CurTexture = TextureManager::LoadTexture(GHOST_IMAGE_PINKY_UP );
    countOfMoves = COUNT_OF_MOVES_AFTER_BFS;
    update( pacmanCoordinates, directionOfPacman );
    go = false;
}