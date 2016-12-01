#include "RobotBehaviorGoalkeaper.h"
#include "Game.h"



RobotBehaviorGoalkeaper::RobotBehaviorGoalkeaper(Behavior _behavior, Robot *_robot, Strategy *_strategy)
    : RobotBehavior ( _behavior, _robot, _strategy )
{
}

double RobotBehaviorGoalkeaper::auxCalculateRobotDistances(double angle)
{
    Point *nextPoint = robot->getPosition ();
    double distPosToGo = robot->getPosition()->getDistanceTo ( robot->getPosToGo() );
    double finalAngle = robot->getPosition()->calculateAngleTo ( robot->getPosToGo() ) + angle;

    strategy->getGameFunctions()->adjustAngle ( finalAngle );
    Point *fPoint = new Point ( robot->getPosition()->getPointX() + distPosToGo * cos ( finalAngle ),
                                robot->getPosition()->getPointY() + distPosToGo * sin ( finalAngle ) );
    vector <double> *distances = new vector <double>;

    while ( nextPoint->getDistanceTo ( fPoint ) > ROBOT_RADIUS )
    {
        this->enemyObstacle ( distances, nextPoint );
        this->teamPartnerObstacle ( distances, nextPoint );
        this->ballObstacle ( distances, nextPoint );
        //this->ourGoalObstacle ( distances, nextPoint );
        nextPoint = new Point ( nextPoint->getPointX() + ROBOT_RADIUS * cos ( finalAngle ),
                                nextPoint->getPointY() + ROBOT_RADIUS * sin ( finalAngle ) );
    }

    if ( distances->size() > 0 )
        return strategy->getGameFunctions()->getObjectNear ( distances );
    return -1;
}

double RobotBehaviorGoalkeaper::calculateMapLinearVelocity()
{
    double velocity = 0.0;
    double distance = robot->getPosition()->getDistanceTo ( robot->getPosToGo() );
    int velocityMax = 4;

    if ( distance > 2000 )
        velocity = velocityMax;
    else if ( distance < strategy->bss->errorDistance )
        velocity = 0;
    else
        velocity = UtilGame::m_map ( distance, 0, 2000, 0.6, velocityMax );

    return velocity;
}
