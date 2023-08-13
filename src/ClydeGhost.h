#ifndef PAC_MAN_CLYDEGHOST_H
#define PAC_MAN_CLYDEGHOST_H

#include <memory>

#include "Ghost.h"



class ClydeGhost : public Ghost
{
public:
    ClydeGhost( std::shared_ptr<Map> map, int chosenDifficultyLevel, int x, int y );
    ~ClydeGhost() = default;

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
    bool chaseMode = false;

    int COUNT_OF_MOVES_AFTER_BFS;
    int m_ChosenDifficultyLevel;
    // if Clyde further than safe length, he starts to chase PACMAN
    int safeLength;

    int distanceBetweenObjects ( std::pair<int,int> object1, std::pair<int,int> object2 ) const;

    int START_POSITION_X;
    int START_POSITION_Y;

    const char * GHOST_IMAGE_CLYDE_UP = "/Users/aleksejgrachev/Desktop/2nd Semester/PA2/PAC-MAN/grachale/assets/ghostOrangeUp.png";
};



#endif //PAC_MAN_CLYDEGHOST_H
