#include "Ghost.h"

#include <math.h>
#include <deque>
#include <queue>

using namespace std;


Ghost::Ghost ( shared_ptr<Map> map )
{
    // full size of ghost
    m_Src . x = m_Src . y = 0;
    m_Src . w = m_Src . h = 30;

    m_Speed = 1;
    m_Map = map;
    countOfMoves = 0;
    m_Direction = 0;

    frigtenGhost = TextureManager::LoadTexture ( FRIGHTEN_GHOST );
    frigtenWhiteGhost = TextureManager::LoadTexture( FRIGHTEN_WHITE_GHOST );

    go = true;
    m_CurTexture = nullptr;
    blinking = false;
}


/**
 * Generating random valid direction for ghost.
 *
 *
 * @return 0 - right, 1 - down, 2 - left, 3 - up
*/
int Ghost::randomDirection() {
    vector<unsigned short int> choiceDirection;
    // check the right
    if (m_Map->m_Map[m_Dest.y][m_Dest.x + m_Speed + 2] == 0 &&
        m_Map->m_Map[m_Dest.y + 2][m_Dest.x + m_Speed + 2] == 0) {
        choiceDirection.push_back(0);
    }
    // check down
    if (m_Map->m_Map[m_Dest.y + m_Speed + 2][m_Dest.x] == 0 &&
        m_Map->m_Map[m_Dest.y + m_Speed + 2][m_Dest.x + 2] == 0) {
        choiceDirection.push_back(1);
    }
    // check the left
    if (m_Map->m_Map[m_Dest.y][m_Dest.x - m_Speed] == 0 &&
        m_Map->m_Map[m_Dest.y + 2][m_Dest.x - m_Speed] == 0) {
        choiceDirection.push_back(2);
    }
    // check up
    if (m_Map->m_Map[m_Dest.y - m_Speed][m_Dest.x] == 0 &&
        m_Map->m_Map[m_Dest.y - m_Speed][m_Dest.x + 2] == 0) {

        choiceDirection.push_back(3);
    }
    // random
    return choiceDirection [rand () % choiceDirection . size ()];
}


/**
 * Movement of ghost according to random direction
 *
 * @param direction direction in which ghost can randomly move
 *
*/
void Ghost::randomPath( int & direction )
{
    switch ( direction ) {
        // right
        case 0:
            if (m_Map->m_Map[m_Dest.y][m_Dest.x + 2 + m_Speed] == 0 &&
                m_Map->m_Map[m_Dest.y + 2][m_Dest.x + 2 + m_Speed] == 0)
                m_Dest.x += m_Speed;
            else
                direction = randomDirection();

            break;
        // down
        case 1:
            if (m_Map->m_Map[m_Dest.y + m_Speed + 2][m_Dest.x] == 0 &&
                m_Map->m_Map[m_Dest.y + m_Speed + 2][m_Dest.x + 2] == 0)
                m_Dest.y += m_Speed;
            else
                direction = randomDirection();

            break;
        // left
        case 2:
            if (m_Map->m_Map[m_Dest.y][m_Dest.x - m_Speed] == 0 &&
                m_Map->m_Map[m_Dest.y + 2][m_Dest.x - m_Speed] == 0)
                m_Dest.x -= m_Speed;
            else
                direction = randomDirection();

            break;
        // up
        case 3:
            if (m_Map->m_Map[m_Dest.y - m_Speed][m_Dest.x] == 0 &&
                m_Map->m_Map[m_Dest.y - m_Speed][m_Dest.x + 2] == 0)
                m_Dest.y -= m_Speed;
            else
                direction = randomDirection();

            break;
        default:
            break;
    }
}

/**
 * Finding a path on map to the certain object using BFS.
 *
 * @param dest coordinates of destination object
 *
 * @return path to the obhect
 *
*/
deque <std::pair<int,int> > Ghost::nextMove( pair < int, int > dest ) {
    // queue of all paths, which we find
    queue<deque<pair<int, int> > > ourQueue;

    // one certain path
    deque<pair<int, int> > path;

    // push back the start point
    path . push_back(make_pair(m_Dest.y, m_Dest.x));

    ourQueue.push(path);

    while (!ourQueue.empty()) {
        path = ourQueue.front();
        ourQueue.pop();
        pair<int, int> last = path [path.size() - 1];

        // if the last coordinates equals dest, we found the path
        if (last == dest) {
            if ( path . size () > 1 ){
                path . pop_front();
            }
            return path;
        }

        // go to right
       if ( m_Map -> m_Map [last . first][last . second + 3] == 0 && m_Map -> m_Map [last . first + 2][last . second + 3] == 0 &&
        std::find ( path . begin (), path . end (), make_pair ( last . first,  last . second + 1 ) ) == path . end () ) {
            deque<pair<int, int> > newPath(path);
            newPath.push_back(make_pair(last.first, last.second + 1));
            ourQueue.push(newPath);
        }
        // go to left
          if (m_Map -> m_Map [last . first ] [last . second - 1] == 0 && m_Map -> m_Map [last . first + 2] [last . second - 1] == 0 &&
            std::find ( path . begin (), path . end (), make_pair ( last . first,  last . second - 1 ) ) == path . end () ) {
                deque<pair<int, int> > newPath(path);
                newPath.push_back(make_pair(last.first, last.second - 1));
                ourQueue.push(newPath);
        }
        // go up
       if (    m_Map -> m_Map [last . first - 1] [last . second] == 0 && m_Map -> m_Map [last . first - 1] [last . second + 2] == 0 &&
                std::find ( path . begin (), path . end (), make_pair ( last . first - 1,  last . second) ) == path . end () )
            {
                deque<pair<int, int> > newPath(path);
                newPath.push_back(make_pair(last.first - 1, last.second ));
                ourQueue.push(newPath);
        }
        // go down
         if (  m_Map -> m_Map [last . first + 3] [last . second] == 0 && m_Map -> m_Map [last . first + 3] [last . second + 2] == 0 &&
                std::find ( path . begin (), path . end (), make_pair ( last . first + 1,  last . second) ) == path . end () )
            {
                deque<pair<int, int> > newPath(path);
                newPath.push_back(make_pair(last.first + 1, last.second));
                ourQueue.push(newPath);
            }
        }
    return path;
}


/**
 * Set the blinking of ghost.
 *
*/
void Ghost::setBlinking ( bool type )
{
    blinking = type;
}

/**
 * Move in random position and change a sking of ghost on frighten.
 *
*/
void Ghost::frighten ( )
{
    // go is a variable to slow down the ghost (2 pacman moves = 1 ghost move)
    if ( go ) {
        randomPath ( m_Direction );
        SDL_Delay (30);
            m_CurTexture = frigtenGhost;
        go = false;
    } else
    {
        if ( blinking )
            m_CurTexture = frigtenWhiteGhost;
        else
            m_CurTexture = frigtenGhost;
        go = true;
    }
}