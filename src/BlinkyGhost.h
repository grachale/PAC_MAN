#ifndef PAC_MAN_BLINKYGHOST_H
#define PAC_MAN_BLINKYGHOST_H

#include <memory>

#include "Ghost.h"


class BlinkyGhost : public Ghost
{
public:
     BlinkyGhost ( std::shared_ptr<Map> map, int chosenDifficultyLevel, int x, int y );
     ~BlinkyGhost () = default;

     virtual void update ( std::pair<int,int> pacmanCoordinates, int directionOfPacman ) override;
     virtual void render () const override;
     virtual void newRound () override;
     virtual void chase (std::pair<int,int> pacmanCoordinates, int directionOfPacman ) override;
     virtual void wasEaten ( ) override;
     virtual void changeChaseMode () override;

     virtual bool getChaseMode () const override;
     virtual std::pair<int,int> getCoordinates () const override;

private:
    // path of the ghost on the map
    std::deque < std::pair<int,int> > m_Trace;

    int m_ChosenDifficultyLevel;
    bool chaseMode = false;

    int COUNT_OF_MOVES_AFTER_BFS;
    const char * GHOST_IMAGE_BLINKY_UP = "/Users/aleksejgrachev/Desktop/2nd Semester/PA2/PAC-MAN/grachale/assets/ghostBlinkyUp.png";

    int START_POSITION_X;
    int START_POSITION_Y;
};


#endif //PAC_MAN_BLINKYGHOST_H
