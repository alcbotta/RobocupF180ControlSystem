#ifndef GAME_H
#define GAME_H

#include "Ball.h"
#include "RobotBehavior.h"
#include "RobotBehaviorGoalkeaper.h"

//#include "Robot.h"

#include "Strategy.h"

#include <iostream>
#include <cstdarg>
#include <vector>

using namespace std;
using namespace Referee;


//namespace SpaceGameSettings
//{
//    enum TeamColor { BLUE, YELLOW };
//    enum FieldSide { LEFT, RIGHT };
//}

//#define REAL_GAME

class FieldSettings
{
    public:
        int fieldHorizontalMin;
        int fieldHorizontalMax;
        int fieldVerticalMin;
        int fieldVerticalMax;
        int goalkeeperAreaRadius;
        int goalkeeperPenalty;
        int penaltyMark;
        int penaltyGoalPoint;
        int goalWidth;
        int robotDistancePenalty;
};

using namespace SpaceGameSettings;
class Game
{
    public:

        FieldSettings *fieldSettings;
        void initFieldSettings ();

        Game ( TeamColor mColor, FieldSide mSide );
        virtual ~Game ();
        virtual void setCmdReferee ( char newCmdReferee );
        virtual char getCmdReferee ();
        virtual char getLastCmdReferee ();
        virtual vector < Robot * > *getRobots ();
        virtual vector < Robot * > *getEnemyRobots ();
        virtual TeamColor getTeamColor ();
        virtual Ball *getBall ();
        virtual Point *getEnemyGoal ();
        virtual Point *getOurGoal ();
        virtual Strategy *getStrategy ();
        FieldSide getSide ();
        virtual void setDetectedRobot ( int RobotId, double posX, double posY, double newOrientation );
        virtual void setDetectedEnemyRobot ( int RobotId, double posX, double posY, double newOrientation );
        virtual void setDetectedBall ( double posX, double posY, double  posZ );
        virtual void initializeGame ();
        virtual void beginStrategy ();
        virtual void changePlayers ();

        virtual void resetEnemyRobots ();
        vector < Robot * > *getRobotsOn ();
        vector < Robot * > *getEnemiesOn ();
        void initGoalPost();
        Point *getEnemyGoalPostUpper();
        Point *getEnemyGoalPostBottom();
        Point *getOutGoalPostUpper();
        Point *getOutGoalPostBottom();
        virtual void setReadyEnemy ( bool ready );
        bool getReadyEnemy ();
        int getPlayersOn ();
        void changeSide ( double &valueToChange );

        void updateRobotBehavior ( Robot *robot, Behavior behavior );

        void showRobotInformation ( Robot *m_robot );
        void showTeam ();

    private:
        Strategy *strategy;
        char cmdReferee;
        char lastCmdReferee;
        vector < Robot *> *robots;
        vector < Robot *> *enemyRobots;
        Ball *ball;
        TeamColor color;
        TeamColor enemyColor;
        FieldSide side;
        Point *goal;
        Point *goalPostUpper;
        Point *goalPostBottom;
        Point *enemyGoal;
        Point *enemyGoalPostUpper;
        Point *enemyGoalPostBottom;
        int playersOn;
        bool readyEnemy;
        int teamSize;
        int enemySize;
};

class GameFunctions
{
    public:
        GameFunctions ( Game *_game );
        vector < Robot * > *getRobotsNearBall ( vector < Robot * > *m_robots);
        vector < Robot * > *getRobotsDif ( vector<Behavior> *behavior );
        vector < Robot * > *getRobotsDif ( Behavior behavior );
        vector < Robot * > *getRobotsDif ( int size, ... );
        vector < Robot * > *getRobotsDifWithGoalk ( int size, ... );
        vector < Robot * > *getRobotsDif ( );

        Point *ballNearRobot ( Robot *kicker, Robot **nRobot );

        void changeBehaviors ( Behavior &behavior1, Behavior &behavior2 );

        double getObjectNear ( vector <double> *distances );

        Robot *getRobot ( Behavior mBehavior );

        bool ballInsideGoalArea ();
        bool pointInsideGoalArea ( Point *point );

        Robot *getGoalKeaper ();
        Robot *getRobotNearToBall ();
        Robot *getRobotNearTo ( Point *point );
        Robot *getRobotNearTo ( Point *point, vector < Robot * > *m_robots );
        Robot *getRobotNearToBall (int size, ... );
        Robot *getRobotNearTo ( Point *point, int size, ... );

        bool ballInsideEnemyGoalArea();
        bool foulForEnemy ();
        bool validatePoint ( Point *point );

//        void calculateRobotDistances ( Robot *robot );
//        double auxCalculateRobotDistances ( Robot *robot, double angle );


        void changeSideAngle ( double &angle );
        bool ballUpperField();
        bool ballIsOnOurSide();
        bool pointIsOnOurSide ( Point *point );
        bool enemyOnOurSide ();

        bool isOutsideField ( Point *point );

        double angleToDist ( double dist );
        double oppositeAngle ( double &angle );
        double reflectedAngle ( double &angle );
        double adjustAngle ( double &angle );

    private:
        Game *game;
};

extern "C"
{
    typedef Game *Game_load_t ( TeamColor color, FieldSide side );
	typedef void Game_unload_t ( Game * );
    Game *getGame ( TeamColor color, FieldSide side );
	void deleteGame ( Game * );
}

#endif
