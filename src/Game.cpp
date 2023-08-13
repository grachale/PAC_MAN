#include "Game.h"

#include <fstream>

#include "Map.h"
#include "Pacman.h"
#include "BlinkyGhost.h"
#include "PinkyGhost.h"
#include "ClydeGhost.h"

using namespace std;


SDL_Renderer * Game::renderer = nullptr;
SDL_Event Game::event;

std::vector< shared_ptr<Ghost> > ghosts;

std::shared_ptr<Map>         map            = nullptr;
std::shared_ptr<Pacman>      player         = nullptr;
std::shared_ptr<BlinkyGhost> redGhost       = nullptr;
std::shared_ptr<ClydeGhost>  orangeGhost    = nullptr;
std::shared_ptr<PinkyGhost>  pinkyGhost     = nullptr;




Game::Game()
{
    m_IsRunning = true;

    // Player has 0 points from the beginning
    m_Score = 0;
    // Player has 3 lives from the beginning
    m_Lives = 3;
    m_DotsEaten = 0;
    m_Time = 0;
    pacmanChases = false;
}

/**
 * Check weather the file exists.
 *
 * @param fileName name of the file
 *
 * @return true (file exists) or false (doesn't exist)
*/
inline bool doesThisFileExist (const std::string & fileName )
{
    ifstream f (fileName . c_str () );
    return f.good();
}
/**
 * Write to the parameter from cin and clear buffer.
 *
 * @param input int which will have the input from cin
 *
*/
inline void cinInputInt ( int & input )
{
    cin >> input;
    cin . clear ();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

/**
 * Initialize the game.
 *
 * @param title name of the window
 * @param xpos x coordinate of the window
 * @param ypos y coordinate of the window
 * @param width width of the window
 * @param heigth height of the window
 * @param fullScreen open the full screen
 * @param loadTheMap load the map from the file
 *
 *
*/
void Game::init ( const char * title, int xpos, int ypos, int width, int height, bool fullScreen, bool loadTheMap )
{
    // setting up flags if fullScreen is TRUE
    int flags = 0;

    if ( fullScreen )
        flags = SDL_WINDOW_FULLSCREEN;

    if ( SDL_Init(SDL_INIT_EVERYTHING) ) {
        m_IsRunning = false;
    } else {
        // creating the window
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        // creating the render
        renderer = SDL_CreateRenderer(window, -1, 0);
        // setting black screen ( red = 0, green = 0, blue = 0 )
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    }

    if ( loadTheMap ) {
        std::string fileName;
        for ( bool isItGoodFile = false; isItGoodFile != true; ) {
            std::cout << "\n  Write a path to your map: \n";
            std::cout << "File must contain a matrix [59][56], difficulty level (0-2), position of pacman ( x, y ) and \npositions of three ghosts in order - RED, PINKY, ORANGE.\n";
            std::cin >> fileName;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if ( doesThisFileExist ( fileName ) ) {
                // file is ok
                map = make_shared<Map>();
                // in case of something
                if (!map->loadMapFromFile ( fileName, m_ChosenDifficultyLevel, PACMAN_POSITION_X, PACMAN_POSITION_Y,
                                            RED_GHOST_POSITION_X, RED_GHOST_POSITION_Y, PINKY_GHOST_POSITION_X, PINKY_GHOST_POSITION_Y,
                                            ORANGE_GHOST_POSITION_X, ORANGE_GHOST_POSITION_Y ) )
                    m_IsRunning = false;
                isItGoodFile = true;
            } else {
                // file is bad
                std::cout << "\nSomething wrong with this file - " << fileName << ".\n";
                int input2;
                do {
                    std::cout << "  1. Try again.  \n  2. Exit. \n";
                    cinInputInt(input2);
                } while (input2 != 1 && input2 != 2);
                // exit was chosen
                if (input2 == 2) {
                    map = make_shared<Map>();
                    isItGoodFile = true;
                    m_IsRunning = false;
                }
            }
        }
    } else
        map = make_shared<Map>(); // default map

    // initialization of player and ghosts
    player       = make_shared<Pacman>      ( map, PACMAN_POSITION_X, PACMAN_POSITION_Y );
    redGhost     = make_shared<BlinkyGhost> ( map, m_ChosenDifficultyLevel, RED_GHOST_POSITION_X, RED_GHOST_POSITION_Y );
    orangeGhost  = make_shared<ClydeGhost>  ( map, m_ChosenDifficultyLevel, ORANGE_GHOST_POSITION_X, ORANGE_GHOST_POSITION_Y );
    pinkyGhost   = make_shared<PinkyGhost>  ( map, m_ChosenDifficultyLevel, PINKY_GHOST_POSITION_X, PINKY_GHOST_POSITION_Y );

    ghosts . push_back ( orangeGhost );
    ghosts . push_back ( pinkyGhost );
    ghosts . push_back ( redGhost );

    if ( m_IsRunning )
        std::cout << "You have 3 lives, let's go! " << std::endl;
}

/**
 * Handle and verify user input, user actions.
 *
*/
void Game::handleEvents ()
{
    SDL_PollEvent ( &event );
    switch ( event . type )
    {
        case SDL_QUIT:
            m_IsRunning = false;
            break;
        default:
            break;
    }
}

/**
 * Update everything ( player, ghosts, map ) what happens in the game.
 *
*/
void Game::update () {

    player -> update(pacmanChases, m_Score, m_DotsEaten, m_Time);

    // if one of the ghost chases PACMAN and game is not in "PACMAN chases" mode
    if ( ( doObjectsIntersect ( player -> getCoordinates(), redGhost -> getCoordinates() ) ||
           doObjectsIntersect ( player -> getCoordinates(), orangeGhost -> getCoordinates() ) ||
           doObjectsIntersect ( player -> getCoordinates(), pinkyGhost -> getCoordinates() ) ) && !pacmanChases )
    {
        // all lives are gone
        if (--m_Lives <= 0) {
            // game over
            std::cout << "You have lost all your lives and earned " << m_Score << " points. Great job!"
                      << std::endl;
            SDL_Delay(1000);
            m_IsRunning = false;
            return;
        } else {
            // some lives are left
            if (m_Lives == 1) {
                std::cout << "1 life left!!!" << std::endl;
            } else {
                std::cout << "2 lives left!" << std::endl;
            }
            player -> newRound();
            for (auto const &ghost: ghosts)
                ghost -> newRound();
            this -> render();
            SDL_Delay(1000);
        }
    }
        // if PACMAN has eaten power dot, he can eat ghosts ( "PACMAN chases" mode )
        if ( pacmanChases ) {
            for ( auto & ghost: ghosts ) {
                // if ghost has not been eaten
                if ( !ghost->getChaseMode() ) {
                    // if PACMAN has eaten the ghost, ghost goes back to its start tile and continues chasing
                    if ( doObjectsIntersect( ghost -> getCoordinates (), player -> getCoordinates () ) ) {
                        ghost -> changeChaseMode () ;
                        ghost -> wasEaten( );
                        m_Score += 200;
                    } else {
                        // if soon the game backs to the "GHOST chases" mode
                        if (m_Time > 100)
                            ghost->setBlinking(true);
                        ghost->frighten();
                    }
                } else {
                    if ( doObjectsIntersect (  ghost -> getCoordinates (),  player -> getCoordinates () ) ) {
                        player -> newRound ();
                        for ( auto & ghost2: ghosts )
                            ghost2 -> newRound();
                        this -> render ();
                        SDL_Delay(1000);
                    }
                    ghost -> update( player -> getCoordinates (), player -> getDirection () );
                }
            }

            m_Time++;

            // if "PACMAN chases" mode is gone
            if ( m_Time > 150 ) {
                for ( auto & ghost: ghosts) {
                    pacmanChases = false;
                    if ( !ghost -> getChaseMode () ) {
                          ghost -> chase(player -> getCoordinates (),  player -> getDirection () );
                          ghost -> setBlinking(false);
                    } else {
                        ghost -> update(player -> getCoordinates (), player -> getDirection () );
                        ghost -> changeChaseMode ();
                    }
                    m_Time = 0;
                }
            }
        // pacman hasn't chosen one of the ghosts
        } else {
            for (auto & ghost: ghosts)
                ghost -> update( player -> getCoordinates(),  player -> getDirection () );
        }
}
/**
 * Render everything ( player, ghosts, map ) on the window.
 *
*/
void Game::render ()
{
    SDL_RenderClear ( renderer );

    player -> render();
    for ( const auto & ghost : ghosts )
        ghost -> render ();
    map -> drawMap();
    SDL_RenderPresent ( renderer );
    // if all dots are eaten new round
    if (m_DotsEaten == map -> m_CountOfDots )
        newLevel ();
}
/**
 * Clean the window and render.
 *
*/
void Game::clean ()
{
    SDL_DestroyWindow   ( window );
    SDL_DestroyRenderer ( renderer );
    SDL_Quit ();
}

/**
 *
 * @return true (the game is running, everything is all right) or false (game is not running)
*/
bool Game::running () const
{
    return m_IsRunning;
}

/**
 * Setting all up for a new match.
 *
*/
void Game::newLevel()
{

    SDL_DestroyWindow   ( window );
    SDL_DestroyRenderer ( renderer );
    SDL_Quit();

    int input;
    do {
        // random choosing the word of congratulation
        std::cout << happyWords [ rand () % ( sizeof ( happyWords ) / sizeof ( happyWords[0] ) ) ] << "!!! You have " << m_Score << " points. \n\n 1. Play next level. \n 2. Exit. \n";
        cinInputInt ( input );
    } while ( input != 1 && input != 2 );

    // next level was selected
    if ( input == 1 )
        this -> init ( "PAC-MAN", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 700, 700, false, false );
    else
        m_IsRunning = false;
}

/**
 * Start menu to communicate with user.
 *
 * @return true (user has chosen to load the map) or false (default map)
*/
bool Game::startMenu()
{
    int input;
    do {
        cout << "PAC-MAN!!!\n\n  1. Play game. \n  2. Load my map. \n  3. Exit. \n ";
        cinInputInt ( input );
    } while ( input != 1 && input != 2 && input != 3 );

    // play game was chosen
    if ( input == 1 )
    {
        int level;
        do {
            cout << "  Select difficulty level:  \n";
            for (unsigned int i = 1; i <= sizeof(difficultyLevel) / sizeof(difficultyLevel[0]); ++i)
                cout << i << ". " << difficultyLevel[i - 1] << std::endl;
            cout << endl;
            cinInputInt ( level );
        } while ( level < 1 || level > int ( sizeof(difficultyLevel) / sizeof(difficultyLevel[0] ) ) );

        m_ChosenDifficultyLevel = level - 1;
    // load the map was chosen
    } else if ( input == 2 )
    {
        return true;
    } else  // exit was chosen
    {
        cout << "Good to see you :) " << endl;
        m_IsRunning = false;
    }
    return false;
}

/**
 * Checks if two objects on the map intersect.
 *
 * @param coordinates1 coordinates of the first object
 * @param coordinates2 coordinates of the second object
 *
 * @return true (intersect) or false (doesn't intersect)
*/
bool Game::doObjectsIntersect ( pair<int,int> coordinates1, pair<int,int> coordinates2 ) const
{
    // left intersection regarding the first object
    if ( coordinates1 . first + 2 == coordinates2 . first && coordinates1. second == coordinates2 . second )
        return true;
    // right intersection regarding the first object
    if ( coordinates1 . first == coordinates2 . first + 2 && coordinates1 . second == coordinates2 . second )
        return true;
    // down intersection regarding the first object
    if ( coordinates1 . first == coordinates2 . first && coordinates1. second + 2 == coordinates2 . second )
        return true;
    // left intersection regarding the first object
    if ( coordinates1 . first == coordinates2 . first && coordinates1. second == coordinates2 . second + 2 )
        return true;

    return false;
}