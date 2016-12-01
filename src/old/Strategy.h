#ifndef STRATEGY_H
#define STRATEGY_H

#include <iostream>
#include <vector>

#include <cstdlib>
#include <ctime>
using namespace std;

class Game;
class GameFunctions;
#include "Robot.h"
#include "UtilGame.h"


using namespace SpaceRobotBehavior;

class BasicStrategySettings
{
    public:
        int kickerDistance;
        int distanceToBall;
        int distanceMinStop;
        int distanceToBallEnemy;
        int errorDistance;
        int robotRadiusError;
        int distanceGoalMin;
        double kickMax;
        double angleMin;
        double angleMinOurSide;
        double forceKickAngle;
        int dangerousRegion1;
        int dangerousRegion2;
        int distMinToOurGoal;
        int attackDistance;
        int strickerDistance;
        int distance_gko_min;
        int distMidfieldAttacking;
        double distToBallAngle;
        double distToBallAngle2Pl;
        double distToBallAngle3Pl;
        double distToBallAngle4Pl;
};

class Strategy
{
    public:

        //Strategy ();
        Strategy ( Game *newGame );
        virtual ~Strategy ();
        virtual void makeStrategy ();
        void setGame ( Game *newGame );
        virtual bool ballInPlay ();

        BasicStrategySettings *bss;
        void initBss ();
        GameFunctions *getGameFunctions ();
        Game *getGame ();

    protected:
        void kickEnemyGoalOrPass ( Robot *robot );
        void positionToTarget ( Robot *robot, Point *position );
        void goToPos ( Robot *robot, Point *position, Point *target );
        void positionToEnemyGoal ( Robot *robot, Point *position );
        void goToValPos ();
        void goToBallKicking ( Robot *robot, Point *target );


        double enemyOnTheLine( Point *bPoint, Point *fPoint );
        vector < Robot * > *getPartners (Robot *robot);

        void behaviorDefender2Pl ( Robot *robot1, Robot *robot2 );
        void behaviorMidfield2Pl ( Robot *robot1, Robot *robot2 );

        void makeRobotShot ( Robot *robot );
        void makeRobotPass ( Robot *robot );

        void strategyStop ();
        void stop2Pl();
        void auxStop1 ( Robot *robot, int distanceForBall  );
        void stop3Pl();
        void posDefGoalPostStop ( Robot *robot1, Robot *robot2 );
        void auxStop2 ( Robot *robot1, Robot *robot2, int distanceToBall );
        void stop4Pl();
        void auxStop3 ( Robot *robot1, Robot *robot2, Robot *robot3 );
        void stop5Pl();
        void stop6Pl();
        void NDefense ( int size, ... );

        double auxDistanceToBall ( bool is_pUpper );
        void auxStopGoal ( Robot *robot );
        void auxStopGoal2Pl ( Robot *robot1, Robot *robot2 );

        vector < Robot * > *enemiesToMark ();
        void markEnemy ( Robot *enemy, Robot *marker );

        void strategyStart ();
        void start2Pl();
        void start3Pl();
        void start4Pl();
        void start5Pl();
        void start6Pl();

        void striker2Pl ( Robot *robot1, Robot *robot2 );
        void positionStriker2Pl ( Robot *robot1, Robot *robot2 );
        void positionStrikerUpper (Robot *robot);
        void positionStrikerBotton ( Robot *robot );


        void strategyDirectKick ();
        void auxStrategyDirectKick ( Robot *robot, Point *target );
        void directKick3Pl ();
        void auxDirectKick3( Robot *robot );
        void directKick4Pl ();
        void directKick5Pl ();
        void directKick6Pl ();

        void strategyIndirectKick ();

        void behaviorGoalkeaper();
        void behaviorGoalkeaperOnly();
        bool ballDangerousRegion ();
        void defenderWall ( Robot *robot1, Robot *robot2 );
        void auxDefenderWall ( Robot *robot1, Robot *robot2 );


        void behaviorGoalkeaperPenalty ();
        void penaltyTeam ();
        void penaltyEnemy ();
        void positionToKick ( Robot *robot );
        void positionToKickToTarget ( Robot *robot );
        void positionToPass ( Robot *kicker, Robot *partner );

        Robot *getEnemyKicker ();
        Robot *getKicker ();
        Robot *getPartner ();


        void posDefensiveGoalPost ( Robot *robot );
        void strategyHalt ();

        void readyPenalty ();
        void readyKickoff ();
        vector <Robot *> *getDangerousEnemies ();
        void strategyKickoff ();

        //variaveis
        Game *game;
        GameFunctions *gameFunctions;
};

#endif
