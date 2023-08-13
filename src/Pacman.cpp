#include "Pacman.h"

using namespace std;

Pacman::Pacman ( shared_ptr<Map> curMap, int x, int y )
{

    textureLeftHalf  = TextureManager::LoadTexture ("assets/pacmanLeftOpenHalf.png");
    textureRigthHalf = TextureManager::LoadTexture ("assets/pacmanRightOpenHalf.png");
    textureUpHalf    = TextureManager::LoadTexture ("assets/pacmanUpOpenHalf.png");
    textureDownHalf  = TextureManager::LoadTexture ("assets/pacmanDownOpenHalf.png");

    textureLeft      = TextureManager::LoadTexture ("assets/pacmanLeft.png");
    textureRigth     = TextureManager::LoadTexture ("assets/pacmanRight.png");
    textureUp        = TextureManager::LoadTexture ("assets/pacmanUp.png");
    textureDown      = TextureManager::LoadTexture ("assets/pacmanDown.png");

    textureFull      = TextureManager::LoadTexture ("assets/pacmanFull.png");

    m_TextureLeft . push_back(textureFull);
    m_TextureLeft . push_back(textureLeftHalf);
    m_TextureLeft . push_back(textureLeft);

    m_TextureRight . push_back(textureFull);
    m_TextureRight . push_back(textureRigthHalf);
    m_TextureRight . push_back(textureRigth);

    m_TextureUp . push_back(textureFull);
    m_TextureUp . push_back(textureUpHalf);
    m_TextureUp . push_back(textureUp);

    m_TextureDown . push_back(textureFull);
    m_TextureDown . push_back(textureDownHalf);
    m_TextureDown . push_back(textureDown);


    m_CurMap = curMap;

    START_POSITION_X = x;
    START_POSITION_Y = y;

    m_Src . x = m_Src . y = 0;
    m_Src . w = m_Src . h = 30;

    m_Dest . x = x;
    m_Dest . y = y;
    m_Dest . w = m_Dest . h = 30;

    frameOfPacman = 0;
}


/**
 * Render the pacman.
 *
*/
void Pacman::render ( )
{
    SDL_Texture * curTexture;
    if ( frameOfPacman == 3 )
        frameOfPacman = 0;

    // controlling of animation of pacman
    switch ( m_Direction )
    {
        case 0:
            curTexture = m_TextureRight[frameOfPacman];
            frameOfPacman++;
            break;
        case 1:
            curTexture = m_TextureDown[frameOfPacman];
            frameOfPacman++;
            break;
        case 2:
            curTexture = m_TextureLeft[frameOfPacman];
            frameOfPacman++;
            break;
        case 3:
            curTexture = m_TextureUp[frameOfPacman];
            frameOfPacman++;
            break;
        case 4:
            curTexture = previous;
            break;
    }
    previous = curTexture;

    SDL_Rect temp = m_Dest;

    temp . x = (temp . x) * 10 + 70;
    temp . y = (temp . y) * 10 + 70;

    TextureManager::Draw ( curTexture, m_Src , temp);
}


/**
 * Update position of the pacman on map according to user input.
 *
 * @param pacmanChases is set if pacman eats power dots
 * @param score sum of points gotten in the game
 * @param dotsTaken count of eaten dots
 * @param time time of "Pacman chase" mode
 *
*/
void Pacman::update ( bool & pacmanChases, int & score, int & dotsTaken, int & time )
{

    // change the m_Direction if key is pressed
    if ( Game::event.type == SDL_KEYDOWN )
    {
        // key was pressed by user
        switch ( Game::event . key . keysym . sym )
        {
            // up
            case SDLK_w:
                m_Direction = 3;
                break;
            // down
            case SDLK_s:
                m_Direction = 1;
                break;
            // left
            case SDLK_a:
                m_Direction = 2;
                break;
            // right
            case SDLK_d:
                m_Direction = 0;
                break;
            default:
                break;
        }
    }
    // move pacman in needed m_Direction
    switch ( m_Direction )
    {
        // right
        case 0:
            // if there is no wall
            if ( checkRight(0,0,0) )
            {
                m_Dest . x += m_Speed;
                break;
            }  else if (checkRight ( 0,2,0 ) ) // if there is a dot
            {
                m_CurMap->m_Map[m_Dest . y + 1][m_Dest . x + m_Speed + 2] = 0;
                m_CurMap->loadMap(m_CurMap->m_Map);
                m_Dest . x += m_Speed;
                score += 10;
                dotsTaken++;
                break;
            }  else if (checkRight ( 3,3,3 ) ) // if there is a portal
            {
                m_Dest . x = 1;
                m_Dest . y = 26;
                break;
            }  else if (checkRight ( 0,4,0 ) ) // if there is a power dot
            {
                m_CurMap->m_Map[m_Dest . y + 1][m_Dest . x + m_Speed + 2] = 0;
                m_Dest . x += m_Speed;
                time = 0;
                pacmanChases = true;
                score += 50;
                break;
            } else // if there is a wall
            {
                m_Direction = 4;
                break;
            }
        // down
        case 1:
            // if there is no wall
            if ( checkDown ( 0, 0, 0 ) )
            {
                m_Dest . y += m_Speed;
                break;
            }
            else if (checkDown ( 0, 2, 0 ) ) // if there is a dot
            {
                m_CurMap->m_Map[m_Dest.y + m_Speed + 2][m_Dest . x + 1] = 0;
                m_Dest . y += m_Speed;
                score += 10;
                dotsTaken++;
                break;
            }
            else if (checkDown ( 0, 4, 0 ) ) // if there is a power dot
            {
                m_CurMap->m_Map[m_Dest.y + m_Speed + 2][m_Dest . x + 1] = 0;
                m_Dest . y += m_Speed;
                pacmanChases = true;
                score += 50;
                time = 0;
                break;
            } else
            {
                m_Direction = 4;
                break;
            }
        // left
        case 2:
            // if there is no wall
            if ( checkLeft ( 0, 0, 0 ) )
            {
                m_Dest . x -= m_Speed;
                break;
            } else if ( checkLeft ( 0, 2, 0 ) ) // if there is a dot
            {
                m_CurMap->m_Map[m_Dest.y + 1][m_Dest . x - m_Speed] = 0;
                m_Dest . x -= m_Speed;
                score += 10;
                dotsTaken++;
                break;
            } else if ( checkLeft ( 3, 3, 3 ) ) // if there is a portal
            {
                m_Dest . x = 52;
                m_Dest . y = 26;
                break;
            } else if ( checkLeft ( 0, 4, 0 ) ) // if there is a power dot
            {
                m_CurMap->m_Map[m_Dest.y + 1][m_Dest . x - m_Speed] = 0;
                m_Dest . x -= m_Speed;
                pacmanChases = true;
                time = 0;
                score += 50;
                break;
            } else
            {
                m_Direction = 4;
                break;
            }
        // up
        case 3:
            // if there is no wall
            if (checkUp ( 0, 0, 0 ) )
            {
                m_Dest . y -= m_Speed;
                break;
            }
            else if ( checkUp ( 0, 2, 0 ) ) // if there is a dot
            {
                m_CurMap->m_Map[m_Dest.y - m_Speed][m_Dest . x + 1] = 0;
                m_Dest.y -= m_Speed;
                score += 10;
                dotsTaken++;
                break;
            } else if  ( checkUp ( 0, 4, 0 ) ) // if there is a power dot
            {
                m_CurMap->m_Map[m_Dest.y - m_Speed][m_Dest . x + 1] = 0;
                m_Dest.y -= m_Speed;
                pacmanChases = true;
                score += 50;
                time = 0;
                break;
            } else
            {
                m_Direction = 4;
                break;
            }
        default:
            break;
    }
    SDL_Delay ( 30 );
}

/**
 * Setting everything up for the new round.
 *
*/
void Pacman::newRound()
{

    m_Dest . x = START_POSITION_X;
    m_Dest . y = START_POSITION_Y;
    m_Direction = 0;

    previous = textureFull;
}


/**
 * Checking the down on accordance with arguments.
 *
 * @param up element on the top
 * @param mid element on medium level
 * @param down element on the groun
 *
 * @return bool (the same) or false (differ)
*/
bool Pacman::checkDown ( int up, int mid, int down ) const
{
  if ( m_CurMap->m_Map[m_Dest.y + m_Speed + 2][m_Dest . x ] == up &&
       m_CurMap->m_Map[m_Dest.y + m_Speed + 2][m_Dest . x + 1 ] == mid &&
       m_CurMap->m_Map[m_Dest.y + m_Speed + 2][m_Dest . x + 2] == down )
      return true;
  return false;
}


/**
 * Checking the up on accordance with arguments.
 *
 * @param up element on the top
 * @param mid element on medium level
 * @param down element on the groun
 *
 * @return bool (the same) or false (differ)
*/
bool Pacman::checkUp ( int up, int mid, int down ) const
{
    if (m_CurMap->m_Map[m_Dest.y - m_Speed][m_Dest . x] == up &&
        m_CurMap->m_Map[m_Dest.y - m_Speed][m_Dest . x + 1] == mid &&
        m_CurMap->m_Map[m_Dest.y - m_Speed][m_Dest . x + 2] == down )
        return true;
    return false;
}


/**
 * Checking the left on accordance with arguments.
 *
 * @param up element on the top
 * @param mid element on medium level
 * @param down element on the groun
 *
 * @return bool (the same) or false (differ)
*/
bool Pacman::checkLeft ( int up, int mid, int down) const
{
    if ( m_CurMap->m_Map[m_Dest.y][m_Dest . x - m_Speed] == up &&
         m_CurMap->m_Map[m_Dest.y + 1][m_Dest . x - m_Speed] == mid &&
         m_CurMap->m_Map[m_Dest. y + 2][m_Dest . x - m_Speed] == down )
        return true;
    return false;
}

/**
 * Checking the right on accordance with arguments.
 *
 * @param up element on the top
 * @param mid element on medium level
 * @param down element on the groun
 *
 * @return bool (the same) or false (differ)
*/
bool Pacman::checkRight ( int up, int mid, int down ) const
{
    if ( m_CurMap->m_Map[m_Dest . y][m_Dest . x + m_Speed + 2] == up &&
         m_CurMap->m_Map[m_Dest . y + 1][m_Dest . x + m_Speed + 2] == mid &&
         m_CurMap->m_Map[m_Dest . y + 2][m_Dest . x + m_Speed + 2] == down )
        return true;
    return false;
}

/**
 * Getting coordinates of the pacman.
 *
 * @return coordinates of pacman
*/
std::pair<int, int> Pacman::getCoordinates() const
{
    return std::make_pair ( m_Dest . x, m_Dest . y );
}

/**
 * Getting direction of the pacman.
 *
 * @return direction of pacman
*/
int Pacman::getDirection() const
{
    return m_Direction;
}