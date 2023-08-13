#include "Map.h"

#include <fstream>

#include "TextureManager.h"



Map::Map ()
{
    // loading needed textures
    m_Tile      = TextureManager::LoadTexture ( "assets/tile_labirinth.png" );
    m_Dot       = TextureManager::LoadTexture ( "assets/dot.png" );
    m_PowerDot  = TextureManager::LoadTexture ( "assets/powerDot.png" );

    // loading default map
    loadMap (m_lvl1 );

    // 2 - dot
    m_CountOfDots = countOfElements( 2 );
    // 4 - power dot
    m_CountOfPowerDots = countOfElements ( 4 );

    m_Src . x = m_Src . y = 0;
    m_Src . h = m_Src . w = m_Dest . h = m_Dest . w = 10;

    m_Dest . x = m_Dest . y = 0;
}


/**
 * Load new map as 2d array.
 *
 * @param map 2d array [59][56]
 */
void Map::loadMap( const int map[COUNT_OF_ROWS_MAP][COUNT_OF_COLUMNS_MAP] ) {
    for ( int row = 0; row < COUNT_OF_ROWS_MAP; ++row )
    {
        for ( int column = 0; column < COUNT_OF_COLUMNS_MAP; ++column )
        {
            m_Map[row][column] = map[row][column];
        }
    }
}

/**
 * Count the certail element on the map.
 *
 * @param type (0 is nothing, 1 is wall, 2 is dot, 3 is portal and 4 is a power dot)
 *
 * @return count of the element of certain type
 */

int Map::countOfElements ( int type )
{
    // amount of any element on map
    int sum = 0;
    for ( int row = 0; row < COUNT_OF_ROWS_MAP; ++row )
    {
        for ( int column = 0; column < COUNT_OF_COLUMNS_MAP; ++column )
        {
            if ( m_Map[row][column] == type )
                sum++;
        }
    }
    return sum;
}

/**
 * Load the map from the given file.
 *
 * @param fileName name of the file, which contains map
 * @param difficultyLevel level of difficulty (from 0 to 2)
 * @param xPacman x coordinate of pacman
 * @param yPacman y coordinate of pacman
 * @param xRedGhost x coordinate of red ghost
 * @param yRedGhost y coordinate of red ghost
 * @param xPinkyGhost x coordinate of pinky ghost
 * @param yPinkyGhost y coordinate of pinky ghost
 * @param xOrangeGhost x coordinate of orange ghost
 * @param yOrangeGhost y coordinate of orange ghost
 *
 * @return true (everything has run smoothly) or false (something wrong has happend during reading the file)
 */
bool Map::loadMapFromFile(std::string fileName, int &difficultyLevel, int &xPacman, int &yPacman, int &xRedGhost,
                          int &yRedGhost, int &xPinkyGhost, int &yPinkyGhost, int &xOrangeGhost, int &yOrangeGhost)
{
    std::ifstream ifs ( fileName );

    if ( ifs . fail () || ! ifs . is_open() )
    {
        ifs . close ();
        return false;
    }
    int mapFromFile [COUNT_OF_ROWS_MAP][COUNT_OF_COLUMNS_MAP];

    int element;
    for ( int row = 0; row < COUNT_OF_ROWS_MAP; ++row )
    {
        for ( int column = 0; column < COUNT_OF_COLUMNS_MAP; ++column )
        {
            if ( !(ifs >> element ) )
            {
                ifs . close ();
                return false;
            }
            mapFromFile[row][column] = element;
        }
    }
    ifs >> difficultyLevel;
    ifs >> xPacman;
    ifs >> yPacman;
    ifs >> xRedGhost;
    ifs >> yRedGhost;
    ifs >> xPinkyGhost;
    ifs >> yPinkyGhost;
    ifs >> xOrangeGhost;
    ifs >> yOrangeGhost;

    ifs . close ();
    loadMap ( mapFromFile );
    return true;
}

/**
 * Draw the loaded map on game's window.
 *
 */
void Map::drawMap()
{
    int type = 0;
    for ( int row = 1; row <= COUNT_OF_ROWS_MAP; ++row )
    {
        for ( int column = 1; column <= COUNT_OF_COLUMNS_MAP; ++column )
        {
            type = m_Map[row - 1][column - 1];
                m_Dest . x = (column - 1) * m_Dest . w + 70;
                m_Dest . y = (row - 1) * m_Dest . h + 70;

            switch (type)
            {
                case 1:
                    TextureManager::Draw ( m_Tile, m_Src, m_Dest );
                    break;
                case 2: {
                    SDL_Rect temp = m_Dest;
                    temp.w = temp.h = 6;
                    TextureManager::Draw(m_Dot, m_Src, temp);
                    break;
                }
                case 4:
                    TextureManager::Draw ( m_PowerDot, m_Src, m_Dest );
                    break;
            }
        }
    }
}
