#ifndef ROBOT_BEHAVIOR_H
#define ROBOT_BEHAVIOR_H

#include "Robot.h"
#include "./src/fuzzy/flie.h"

//#define DONT_KICK_Z
#define MIN_VEL_FUZZY 0.0

class Strategy;
class RobotBehavior
{

public:
    RobotBehavior ( Behavior _behavior, Robot *_robot, Strategy *_strategy);
    virtual ~RobotBehavior ();

    virtual void calculateVelocities ();
    virtual bool notCallFuzzy ();

    virtual void fuzzyLinearVelocity ();
    virtual void mapVelocities ();
    virtual void mapLinearVelocity ();
    virtual void mapAngularVelocity ();
    virtual double calculateMapLinearVelocity ();
    virtual double calculateMapAngularVelocity ();

    virtual double calculateMapKickZ ();//
    virtual double calculateMapKickWithZ ();
    virtual double calculateMapKick ();
    virtual double calculateMapKickZPartner ();
    virtual double calculateMapKickWithZPartner ();
    virtual double calculateMapKickToPartner ();

    virtual Point *ballInterception();

    virtual double auxCalculateRobotDistances ( double angle );

    virtual void enemyObstacle ( vector <double> *distances, Point *nextPoint );
    virtual void teamPartnerObstacle ( vector <double> *distances, Point *nextPoint );
    virtual void ballObstacle ( vector <double> *distances, Point *nextPoint );
    virtual void ourGoalObstacle ( vector <double> *distances, Point *nextPoint );

    flie *getObjFLie ();

    const Behavior getBehavior();

protected:
    const Behavior BEHAVIOR;
    Strategy *strategy;
    Robot *robot;
    flie *objFlie;
    float rangeHigh;
};

#endif
