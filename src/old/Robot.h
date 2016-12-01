#ifndef ROBOT_H
#define ROBOT_H

#include <string>
using std::string;
#include "Element.h"
#include <vector>
#include "UtilGame.h"

namespace SpaceRobotBehavior
{
    enum  Behavior
    {
        GOALKEAPER, // 0
        GOALKEAPER_ONLY, // 1
        DEFENDER_ONLY, // 2
        DEFENDER_LEFT, // 3
        DEFENDER_RIGHT, // 4
        MIDFIELD_ONLY, // 5
        MIDFIELD_LEFT, // 6
        MIDFIELD_RIGHT, // 7
        STRIKER_ONLY, // 8
        STRIKER_LEFT, // 9
        STRIKER_RIGHT, // 10
        OFF, //11
        ENEMY  // 12
    };
}

class RobotBehavior;
class CommunicationSystem;
class ComSysSimulator;

using namespace SpaceGameSettings;
using namespace SpaceRobotBehavior;

class Robot : public Element
{
    public:
        Robot ( int id, TeamColor *_color, int _number, CommunicationSystem *_comSystem );
        //Robot ( int id, RobotBehavior _robotBehavior, double newOrientation = 0 );
        TeamColor getColor();
        const int getNumber ();
        void setOrientation ( double newOrientation );
        double getOrientation ();
        double calculateAngleTo ( Point *anotherPoint );
        void setKick ( double newKick );
        virtual double getKick ();
        void setDribble ( double newDribble );
        virtual double getDribble ();
        //void setBehavior ( Behavior newBehavior );
        void setRobotBehavior ( RobotBehavior *_robotBehavior );
        RobotBehavior *getRobotBehavior ();
        Behavior getBehavior();
        virtual const int getId ();
        void setAngularVelocity ( double angVelocity );
        virtual double getAngularVelocity ();
        virtual void setIsOn ( bool on );
        virtual bool getIsOn ();
        virtual string getBehaviorStr ();
        virtual void resetRobot ();
        void addMark ( Robot *robot );
        void clearIsMarkedBy ();
        vector <Robot *> *getIsMarkedBy();
        void setPossessionBall ( bool poss );
        bool getPossessionBall ();
        void showInformation ();
        void setPosToGo ( Point *pos );
        Point *getPosToGo ();
        bool isReady();
        void setGoingToBall ( bool go );
        bool getGoingToBall ();
        double *getDistances ();
        void setTarget ( Point *_target );
        Point *getTarget ();

        bool foundObstacle ();

        void execCommand ();

        void calculateLinearVelocity ();
        void calculateAngularVelocity ();
        void calculateVelocities ();

        void calculateDistances ();

        void calculateKickZ ();
        void calculateKickWithZ ();
        void calculateKick ();
        void calculateKickZPartner ();
        void calculateKickWithZPartner ();
        void calculateKickToPartner ();

        enum RobotDirections { DIR_FORWARD, DIR_LEFT, DIR_RIGHT };
        const double ANGLE_DIRECTION;

    private:

        //void updateRobotBehavior ( Behavior _behavior );

        bool goingToBall;
        Point *target;
        Point *posToGo;
        double angularVelocity;
        //Behavior behavior;
        RobotBehavior *robotBehavior;
        double kick;
        double dribble;
        double orientation;
        bool isOn;
        double offTime1;
        vector <Robot *> *isMarkedBy;
        bool possessionBall;
        double *distances;
        string *behaviorStr;

        const int ID;
        const int NUMBER;
        TeamColor *color;
        CommunicationSystem *comSystem;
};
#endif
