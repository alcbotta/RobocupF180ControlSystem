#ifndef ROBOT_BEHAVIOR_GOALKEAPER
#define ROBOT_BEHAVIOR_GOALKEAPER

#include "RobotBehavior.h"

class RobotBehaviorGoalkeaper : public RobotBehavior
{

public:

    RobotBehaviorGoalkeaper ( Behavior _behavior, Robot *_robot, Strategy *_strategy);

    //sobrepoe implementacao da super classe para remover a area do goleiro do calculo
    virtual double auxCalculateRobotDistances ( double angle );
    virtual double calculateMapLinearVelocity ();
};

#endif
