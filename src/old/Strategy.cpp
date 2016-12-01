#include "Game.h"
#include "Strategy.h"

#include <QDebug>

//#define FUZZY_TEST

void Strategy::initBss()
{
    bss = new BasicStrategySettings;

    bss->kickerDistance = 120;
    int distMin = 500;
    bss->distanceToBall = distMin + ROBOT_RADIUS;
    bss->errorDistance = 15;
    bss->robotRadiusError = ROBOT_RADIUS + bss->errorDistance;
    bss->distanceGoalMin = game->fieldSettings->goalWidth / 2 + 2 * ROBOT_RADIUS;
    bss->kickMax = 8.0;
    bss->angleMin = 3.5;
    bss->angleMinOurSide = 5.0;
    bss->forceKickAngle = 20.0;
    bss->dangerousRegion1 = -1500;
    bss->dangerousRegion2 = -2500;
    bss->distMinToOurGoal= game->fieldSettings->goalkeeperAreaRadius + bss->robotRadiusError;
    bss->attackDistance = 900 + ROBOT_RADIUS;
    bss->distance_gko_min = 2 * bss->robotRadiusError;
    bss->distToBallAngle = atan2 ( ROBOT_RADIUS, bss->distanceToBall );
    bss->distToBallAngle2Pl = atan2 ( 2.2 * ROBOT_RADIUS, bss->distanceToBall );
    bss->distToBallAngle3Pl = atan2 ( 4.7 * ROBOT_RADIUS, bss->distanceToBall );
    bss->distToBallAngle4Pl = atan2 ( 8 * ROBOT_RADIUS, bss->distanceToBall );
    bss->strickerDistance = distMin + ROBOT_RADIUS;
    bss->distanceToBallEnemy = bss->distanceToBall + ROBOT_RADIUS;
    bss->distMidfieldAttacking = 1400 + ROBOT_RADIUS;
    bss->distanceMinStop = 1800;
}

Strategy::Strategy ( Game *newGame )
{
    game = newGame;
    gameFunctions = new GameFunctions ( game );
    initBss();
}

GameFunctions *Strategy::getGameFunctions()
{
    return gameFunctions;
}

Game *Strategy::getGame()
{
    return game;
}

Strategy::~Strategy ()
{

}

//executa estrategia com base no comando do juiz
void Strategy::makeStrategy ()
{

    //behaviorGoalkeaper();
    //cout << "makeStrategy::game->getCmdReferee() = " << game->getCmdReferee() << endl;
    //cout << "distancia = " << game->getBall()->getPosition()->getDistanceTo ( game->getOurGoal() ) << endl;
    //cout << "game->getReadyEnemy() = " << game->getReadyEnemy() << endl;
    switch ( game->getCmdReferee() )
    {
        case Stop:
        {
            strategyStop();
            break;
        }
        case Ready:
        {

            char lastCmd = game->getLastCmdReferee();
            if ( gameFunctions->foulForEnemy() )
                game->setReadyEnemy ( true );

            if ( game->getTeamColor() == YELLOW )
            {
                if ( lastCmd == PenaltyYellow )
                {
                    readyPenalty();
                }
                else if ( lastCmd == KickOffYellow )
                {
                    readyKickoff ();
                }
                else
                {
                    if ( lastCmd == PenaltyBlue )
                        penaltyEnemy ();
                    else
                    {
                        strategyStart();
                    }
                }
            }
            else
            {
                if ( lastCmd == PenaltyBlue )
                {
                    readyPenalty();
                }
                else if ( lastCmd == KickOffBlue )
                {
                    readyKickoff ();
                }
                else
                {
                    if ( lastCmd == PenaltyYellow )
                        penaltyEnemy ();
                    else
                    {
                        strategyStart();
                    }
                }
            }
            break;
        }
        case Start:
        {
            strategyStart();
            break;
        }

        case KickOffYellow:
        {
            if ( game->getTeamColor() == YELLOW )
            {
                strategyKickoff ();
            }
            else
            {
                strategyStop ();
            }
            break;
        }
        case PenaltyYellow :
        {
            if ( game->getTeamColor() == YELLOW )
            {
                penaltyTeam();
            }
            else
            {
                penaltyEnemy();
            }
            break;
        }
        case DirectFreeKickYellow:
        {
            if ( game->getTeamColor() == YELLOW )
            {
                strategyDirectKick ();
            }
            else
            {
                strategyStop();
            }
            break;
        }
        case IndirectFreeKickYellow:
        {
            if ( game->getTeamColor() == YELLOW )
            {
                strategyIndirectKick();
            }
            else
            {
                strategyStop();
            }
            break;
        }
        case KickOffBlue:
        {
            if ( game->getTeamColor() == BLUE )
            {
                strategyKickoff ();
            }
            else
            {
                strategyStop ();
            }
            break;
        }
        case PenaltyBlue :
        {
            if ( game->getTeamColor() == BLUE )
            {
                penaltyTeam();
            }
            else
            {
                penaltyEnemy();
            }
            break;
        }
        case DirectFreeKickBlue:
        {
            if ( game->getTeamColor() == BLUE )
            {
                strategyDirectKick ();
            }
            else
            {
                strategyStop();
            }
            break;
        }
        case IndirectFreeKickBlue:
        {
            if ( game->getTeamColor() == BLUE )
            {
                strategyIndirectKick();
            }
            else
            {
                strategyStop();
            }
            break;
        }
        case Halt:
        {
            strategyHalt ();
            break;
        }
        case TimeoutBlue:
        {
            break;
        }
        case TimeoutEndBlue:
        {
            break;
        }
        case GoalScoredBlue:
        {
            break;
        }
        case DecreaseGoalScoredBlue:
        {
            break;
        }
        case YellowCardBlue:
        {
            break;
        }
        case RedCardBlue:
        {
            break;
        }
        case TimeoutYellow:
        {
            break;
        }
        case TimeoutEndYellow:
        {
            break;
        }
        case GoalScoredYellow:
        {
            break;
        }
        case DecreaseGoalScoredYellow:
        {
            break;
        }
        case YellowCardYellow:
        {
            break;
        }
        case RedCardYellow:
        {
            break;
        }
        case BeginFirstHalf:
        {
            strategyStop();
            break;
        }
        case BeginHalfTime:
        {
            break;
        }
        case BeginSecondHalf:
        {
            strategyStop();
            break;
        }
        case BeginOvertimeHalf1:
        {
            break;
        }
        case BeginOvertimeHalf2:
        {
            break;
        }
        case Cancel:
        {

            break;
        }
    }
}

void Strategy::strategyStart ()
{
    behaviorGoalkeaper();
    switch ( game->getPlayersOn() )
    {
        case 2:
            start2Pl();
            break;
        case 3:
            start3Pl();
            break;
        case 4:
            start4Pl();
            break;
        case 5:
            start5Pl();
            break;
        case 6:
            start6Pl();
            break;
    }
}

void Strategy::start2Pl()
{
    if ( gameFunctions->ballInsideGoalArea() )
    {
        vector < Robot * > *enemies = enemiesToMark();
        if ( enemies->size() > 0 )
        {
            markEnemy( enemies->at( 0 ), gameFunctions->getRobot( DEFENDER_ONLY ) );
        }
        else
        {
            auxDirectKick3 ( gameFunctions->getRobot( DEFENDER_ONLY ) );
        }
    }
    else
    {
        kickEnemyGoalOrPass ( gameFunctions->getRobot( DEFENDER_ONLY ) );
    }
}

void Strategy::start3Pl()
{
    Robot *robot1 = gameFunctions->getRobot ( MIDFIELD_LEFT );
    Robot *robot2 = gameFunctions->getRobot ( MIDFIELD_RIGHT );

    if ( gameFunctions->ballIsOnOurSide() && gameFunctions->enemyOnOurSide() )
        behaviorMidfield2Pl ( robot1, robot2 );
    else
        striker2Pl ( robot1, robot2 );

}


void Strategy::behaviorDefender2Pl ( Robot *robot1, Robot *robot2 )
{
    auxDefenderWall( robot1, robot2 );
    if ( ballDangerousRegion() )
    {
        goToValPos ();
    }
}

void Strategy::behaviorMidfield2Pl ( Robot *robot1, Robot *robot2 )
{
    auxStopGoal2Pl ( robot1, robot2 );
    goToValPos ();
}

void Strategy::defenderWall ( Robot *robot1, Robot *robot2 )
{
    auxDefenderWall( robot1, robot2 );
}

void Strategy::auxDefenderWall ( Robot *robot1, Robot *robot2 )
{
    double angle = game->getOurGoal()->calculateAngleTo ( game->getBall()->getPosition() );
    double angleDist = gameFunctions->angleToDist ( bss->distMinToOurGoal );

    Point *position = new Point (     game->getOurGoal()->getPointX() + bss->distMinToOurGoal * cos ( angle ),
                                    game->getOurGoal()->getPointY() + bss->distMinToOurGoal* sin ( angle ) );

    positionToTarget( robot1,  position );
    position = new Point (  game->getOurGoal()->getPointX() + bss->distMinToOurGoal* cos ( angle + angleDist ),
                            game->getOurGoal()->getPointY() + bss->distMinToOurGoal* sin ( angle + angleDist ) );
    positionToTarget( robot2,  position );
}

void Strategy::start4Pl()
{
    if ( gameFunctions->ballIsOnOurSide() )
    {
        Robot *robot = gameFunctions->getRobotNearToBall();
        if ( gameFunctions->enemyOnOurSide() )
        {
            vector <Robot *> *robotsDif = gameFunctions->getRobotsDif ( robot->getBehavior() );
            auxStopGoal2Pl ( robotsDif->at ( 0 ), robotsDif->at ( 1 ) );
            kickEnemyGoalOrPass( robot );
        }
        else
        {
            if ( robot->getBehavior() == DEFENDER_ONLY )
            {
                positionStriker2Pl ( gameFunctions->getRobot( MIDFIELD_LEFT ), gameFunctions->getRobot( MIDFIELD_RIGHT ) );
                kickEnemyGoalOrPass( robot );
            }
            else
            {
                striker2Pl ( gameFunctions->getRobot( MIDFIELD_LEFT ), gameFunctions->getRobot( MIDFIELD_RIGHT ) );
                auxStopGoal( gameFunctions->getRobot( DEFENDER_ONLY ) );
            }
        }
    }
    else
    {
        striker2Pl ( gameFunctions->getRobot( MIDFIELD_LEFT ), gameFunctions->getRobot( MIDFIELD_RIGHT ) );
        //auxStopGoal( gameFunctions->getRobot( DEFENDER_ONLY ) );
        posDefensiveGoalPost ( gameFunctions->getRobot( DEFENDER_ONLY ) );
    }
}

void Strategy::start5Pl()
{
    this->auxStopGoal2Pl ( gameFunctions->getRobot ( DEFENDER_LEFT ), gameFunctions->getRobot ( DEFENDER_RIGHT ) );

    Robot *robot = gameFunctions->getRobotNearToBall();
    Robot *anRobot = NULL;
    if ( robot->getBehavior() == STRIKER_ONLY )
        anRobot = gameFunctions->getRobot ( MIDFIELD_ONLY );
    else if ( robot->getBehavior() == MIDFIELD_ONLY )
        anRobot = gameFunctions->getRobot ( STRIKER_ONLY );

    if ( anRobot == NULL )
    {
        positionStriker2Pl ( gameFunctions->getRobot( STRIKER_ONLY ), gameFunctions->getRobot( MIDFIELD_ONLY ) );
    }
    else
    {
        auxDirectKick3 ( anRobot );
    }
    kickEnemyGoalOrPass( robot );

//    if ( gameFunctions->ballIsOnOurSide() )
//    {
//        Robot *robot = gameFunctions->getRobotNearToBall();
//    }
//    else
//    {
//        //behaviorDefender2Pl ( gameFunctions->getRobot ( DEFENDER_LEFT ), gameFunctions->getRobot ( DEFENDER_RIGHT ) );
//        striker2Pl ( gameFunctions->getRobot( STRIKER_ONLY ), gameFunctions->getRobot( MIDFIELD_ONLY  ) );
//    }
}

void Strategy::start6Pl()
{
    this->auxStopGoal2Pl ( gameFunctions->getRobot ( DEFENDER_LEFT ), gameFunctions->getRobot ( DEFENDER_RIGHT ) );
    Robot *robot = gameFunctions->getRobotNearToBall ();

    if ( robot->getBehavior() == DEFENDER_LEFT || robot->getBehavior() == DEFENDER_RIGHT )
    {
        positionStriker2Pl ( gameFunctions->getRobot( STRIKER_LEFT ), gameFunctions->getRobot( STRIKER_RIGHT ) );
        double angle;
        if ( gameFunctions->ballUpperField() )
            angle = game->getEnemyGoalPostBottom()->calculateAngleTo( game->getBall()->getPosition() );
        else
            angle = game->getEnemyGoalPostUpper()->calculateAngleTo( game->getBall()->getPosition() );
        Point *position = new Point ( game->getEnemyGoal()->getPointX() + bss->distMidfieldAttacking * cos ( angle ),
                                    game->getEnemyGoal()->getPointY() + bss->distMidfieldAttacking * sin ( angle ) );
        positionToTarget( gameFunctions->getRobot( MIDFIELD_ONLY ), position );
    }
    else
    {
        vector <Behavior> *behaviors = new vector <Behavior>;
        behaviors->push_back ( robot->getBehavior() );
        behaviors->push_back ( DEFENDER_LEFT );
        behaviors->push_back ( DEFENDER_RIGHT );
        vector <Robot *> *robots = gameFunctions->getRobotsDif ( behaviors );
        positionStriker2Pl ( robots->at ( 0 ), robots->at ( 1 ) );

    }
    kickEnemyGoalOrPass( robot );

    //------------------------//----------------------
//    if ( ballDangerousRegion() )
//    {
//        behaviorDefender2Pl ( gameFunctions->getRobot ( DEFENDER_LEFT ), gameFunctions->getRobot ( DEFENDER_RIGHT ) );
//        positionStriker2Pl ( gameFunctions->getRobot( STRIKER_LEFT ), gameFunctions->getRobot( STRIKER_RIGHT ) );
//        vector < Robot * > *enemies = enemiesToMark();
//        if ( enemies->size() > 0 )
//        {
//            markEnemy( enemies->at( 0 ), gameFunctions->getRobot( MIDFIELD_ONLY ) );
//        }
//        else
//        {
//            double angle;
//            if ( gameFunctions->ballUpperField() )
//                angle = game->getEnemyGoalPostBottom()->calculateAngleTo( game->getBall()->getPosition() );
//            else
//                angle = game->getEnemyGoalPostUpper()->calculateAngleTo( game->getBall()->getPosition() );
//            Point *position = new Point ( game->getEnemyGoal()->getPointX() + bss->distMidfieldAttacking * cos ( angle ),
//                                        game->getEnemyGoal()->getPointY() + bss->distMidfieldAttacking * sin ( angle ) );
//            positionToTarget( gameFunctions->getRobot( MIDFIELD_ONLY ), position );
//        }
//    }
//    else
//    {
//        this->auxStopGoal2Pl ( gameFunctions->getRobot ( DEFENDER_LEFT ), gameFunctions->getRobot ( DEFENDER_RIGHT ) );
//        Robot *robot = gameFunctions->getRobotNearToBall( 3,   gameFunctions->getRobot ( MIDFIELD_ONLY ),
//                                                            gameFunctions->getRobot ( STRIKER_LEFT ),
//                                                            gameFunctions->getRobot ( STRIKER_RIGHT ) );
//        vector <Behavior> *behaviors = new vector <Behavior>;
//        behaviors->push_back ( robot->getBehavior() );
//        behaviors->push_back ( DEFENDER_LEFT );
//        behaviors->push_back ( DEFENDER_RIGHT );
//        vector <Robot *> *robots = gameFunctions->getRobotsDif ( behaviors );
//        positionStriker2Pl ( robots->at ( 0 ), robots->at ( 1 ) );
//        kickEnemyGoalOrPass( robot );
//    }
}

void Strategy::striker2Pl( Robot *robot1, Robot *robot2 )
{
    Robot *robot = this->gameFunctions->getRobotNearToBall();
    kickEnemyGoalOrPass ( robot );
    if ( robot->getBehavior() == robot1->getBehavior() )
        auxDirectKick3 ( robot2 );
    else
        auxDirectKick3 ( robot1 );
}

void Strategy::positionStriker2Pl ( Robot *robot1, Robot *robot2 )
{
    double angle = game->getBall()->getPosition()->calculateAngleTo ( game->getEnemyGoal() );

    gameFunctions->oppositeAngle ( angle );
    Point *position;
    if ( abs ( angle * 180 / PI ) < 10 )
    {
        angle = abs ( angle * 180 / PI );
        angle = UtilGame::m_map ( angle, 0, 10, 170, 150 ) * PI / 180;
        gameFunctions->oppositeAngle ( angle );
        position = new Point (    game->getEnemyGoalPostUpper()->getPointX() + bss->strickerDistance * cos ( angle ),
                                game->getEnemyGoalPostUpper()->getPointY() + bss->strickerDistance * sin ( angle ) );
        positionToTarget( robot1, position );

        position = new Point (    game->getEnemyGoalPostBottom()->getPointX() + bss->strickerDistance * cos ( -angle ),
                                game->getEnemyGoalPostBottom()->getPointY() + bss->strickerDistance * sin ( -angle ) );
        positionToTarget( robot2, position );
    }
    else
    {
        double dist;
        double copiedAngle = angle;
        angle = abs ( angle * 180 / PI );
        dist =  UtilGame::m_map ( angle, 10, 90, bss->distMidfieldAttacking, bss->strickerDistance  );
        angle = UtilGame::m_map ( angle, 10, 90, 160, 200 ) * PI / 180;
        gameFunctions->adjustAngle ( angle );
        gameFunctions->changeSideAngle ( angle );
        position = new Point (    game->getEnemyGoal()->getPointX() + dist * cos ( angle ),
                                game->getEnemyGoal()->getPointY() + dist * sin ( angle ) );
        positionToTarget( robot1, position );

        angle = copiedAngle;
        angle = abs ( angle * 180 / PI );
        dist = UtilGame::m_map ( angle, 10, 90, game->fieldSettings->goalWidth / 2 + ROBOT_RADIUS, game->fieldSettings->goalWidth / 2 + 3 * ROBOT_RADIUS  );
        //dist = game->fieldSettings->goalWidth / 2 + ROBOT_RADIUS;
        angle = UtilGame::m_map ( angle, 10, 90, 100, 140 ) * PI / 180;
        gameFunctions->changeSideAngle ( angle );
        position = new Point (    game->getEnemyGoal()->getPointX() + dist * cos ( angle ),
                                game->getEnemyGoal()->getPointY() + dist * sin ( angle ) );
        positionToTarget( robot2, position );
    }
}



//VAI PARA GAME ???
vector <Robot *> *Strategy::enemiesToMark()
{
    vector < Robot * > *robots = new vector < Robot * >;
    vector < Robot * > *enemies = game->getEnemyRobots();

    for ( int i = 0; i < enemies->size(); i++ )
    {
        double x = enemies->at( i )->getPosition()->getPointX();
        game->changeSide ( x );
        if ( x < 0 ) //robô adversário está do nosso lado do campo
        {
            if ( !gameFunctions->pointInsideGoalArea( enemies->at ( i )->getPosition() ) )
            {
                if  ( !gameFunctions->isOutsideField ( enemies->at ( i )->getPosition() ) )
                    robots->push_back( enemies->at ( i ) );
            }
        }
    }
    return robots;
}

void Strategy::strategyIndirectKick()
{
    behaviorGoalkeaper();
    if ( game->getPlayersOn() == 1 )
        return;

    Robot *kicker = getKicker();
    Robot *partner = getPartner();

    if ( partner == NULL )
    {
        auxStop1 ( kicker, bss->distanceToBall );
    }
    else
    {
        positionToPass ( kicker, partner );

        if ( kicker->isReady() )
        {

            if ( partner->isReady() )
            {
                partner->showInformation();
                goToBallKicking ( kicker, partner->getPosition() );
            }
        }
    }
    switch ( game->getPlayersOn() )
    {
        case 3:
            directKick3Pl();
            break;
        case 4:
            directKick4Pl();
            break;
        case 5:
            directKick5Pl();
            break;
        case 6:
            directKick6Pl();
            break;
    }
}

void Strategy::positionToPass(Robot *kicker, Robot *partner)
{
    double angle = game->getBall()->getPosition()->calculateAngleTo ( partner->getPosToGo() );
    Point *position =
            new Point ( game->getBall()->getPosition()->getPointX() - bss->robotRadiusError * cos ( angle ),
                        game->getBall()->getPosition()->getPointY() - bss->robotRadiusError * sin ( angle ) );
    positionToTarget ( kicker, position );
}


void Strategy::strategyDirectKick ()
{
    //if ( game->getPlayersOn() > 1 )
    behaviorGoalkeaper();

    Robot *robot = getKicker () ;
    positionToKick ( robot );
    goToBallKicking ( robot, game->getEnemyGoal() );

    switch ( game->getPlayersOn() )
    {
        case 3:
            directKick3Pl();
            break;
        case 4:
            directKick4Pl();
            break;
        case 5:
            directKick5Pl();
            break;
        case 6:
            directKick6Pl();
            break;
    }
}

bool Strategy::ballInPlay ()
{
    if ( game->getBall()->getVelocity()->getModule () > game->getBall()->ballSettings->ballVelocityMin )
        return true;
    return false;
}

void Strategy::directKick3Pl ()
{
    auxDirectKick3 ( this->getPartner() );
}

void Strategy::auxDirectKick3( Robot *robot )
{
    double angle = game->getBall()->getPosition()->calculateAngleTo ( game->getEnemyGoal() );
    gameFunctions->oppositeAngle ( angle );
    Point *position;
    if ( abs ( angle * 180 / PI ) < 10 )
    {

        angle = abs ( angle * 180 / PI );
        angle = UtilGame::m_map ( angle, 0, 10, 170, 150 ) * PI / 180;
        gameFunctions->oppositeAngle ( angle );
        Point *point;
        if ( gameFunctions->ballUpperField() )
        {
            gameFunctions->reflectedAngle ( angle )   ;
            point = game->getEnemyGoalPostBottom();
        }
        else
            point = game->getEnemyGoalPostUpper();


        position = new Point (    point->getPointX() + bss->strickerDistance * cos ( angle ),
                                point->getPointY() + bss->strickerDistance * sin ( angle ) );
        positionToTarget( robot, position );
    }
    else if ( abs ( angle * 180 / PI ) >= 40 )
    {
        angle = abs ( angle * 180 / PI );
        angle = UtilGame::m_map ( angle, 40, 90, 110, 180 ) * PI / 180;
        gameFunctions->oppositeAngle ( angle );
        if ( gameFunctions->ballUpperField() )
            gameFunctions->reflectedAngle ( angle )   ;
        position = new Point (    game->getEnemyGoal()->getPointX() + bss->strickerDistance * cos ( angle ),
                                game->getEnemyGoal()->getPointY() + bss->strickerDistance * sin ( angle ) );
        positionToTarget( robot, position );
    }
    else
    {
        angle = abs ( angle * 180 / PI );
        angle = UtilGame::m_map ( angle, 10, 40, 140, 110 ) * PI / 180;
        gameFunctions->oppositeAngle ( angle );
        if ( gameFunctions->ballUpperField() )
            gameFunctions->reflectedAngle ( angle )   ;
        position = new Point (    game->getEnemyGoal()->getPointX() + bss->strickerDistance * cos ( angle ),
                                game->getEnemyGoal()->getPointY() + bss->strickerDistance * sin ( angle ) );
        positionToTarget( robot, position );
    }
}

void Strategy::directKick4Pl ()
{
    auxStopGoal( gameFunctions->getRobot( DEFENDER_ONLY ) );
    auxDirectKick3( getPartner() );
}

void Strategy::directKick5Pl ()
{
    auxDirectKick3( gameFunctions->getRobot( STRIKER_ONLY ) );
    auxStopGoal2Pl ( gameFunctions->getRobot( DEFENDER_LEFT ), gameFunctions->getRobot( DEFENDER_RIGHT ) );
}


void Strategy::directKick6Pl ()
{
    auxStopGoal2Pl ( gameFunctions->getRobot( DEFENDER_LEFT ), gameFunctions->getRobot( DEFENDER_RIGHT ) );
    positionStriker2Pl ( gameFunctions->getRobot( STRIKER_LEFT ), gameFunctions->getRobot( STRIKER_RIGHT ) );
}

void Strategy::strategyKickoff ()
{
    if ( game->getPlayersOn() > 1 )
    {
        behaviorGoalkeaper();
        strategyStop ();
    }

    positionToKick ( getKicker () );
}

void Strategy::readyKickoff ()
{
    srand ( time ( NULL ) );
    Robot *robot = getKicker ();
    Point *target = game->getEnemyGoal();
    robot->setTarget ( target );
    robot->calculateAngularVelocity();

    if ( robot->getAngularVelocity() == 0.0 )
    {
        robot->setPosToGo ( target );
        robot->calculateLinearVelocity();
        robot->calculateKickWithZ();
        robot->calculateKickZ();
    }
    robot->execCommand();
}

void Strategy::readyPenalty ()
{
    srand ( time ( NULL ) );
    Robot *robot = getKicker ();
    double y = game->fieldSettings->penaltyGoalPoint;
    double x = game->getEnemyGoal()->getPointX();
    if ( rand() % 2 )
        y *= -1;

    Point *target = new Point ( x, y );

    robot->setTarget ( target );
    robot->calculateAngularVelocity();

    if ( robot->getAngularVelocity() == 0.0 )
    {
        robot->setPosToGo ( target );
        robot->calculateLinearVelocity();
        robot->setKick ( bss->kickMax );
    }
    robot->execCommand();

//    Point *target = new Point ( x, y );

//    robot->setTarget ( target );
//    this->positionToKickToTarget ( robot );

////    if ( robot->getAngularVelocity() == 0.0 )
////    {
////        robot->setPosToGo ( target );
////        robot->calculateLinearVelocity();
////        robot->setKick ( bss->kickMax );
////    }
//    robot->execCommand();
}


void Strategy::strategyHalt ()
{
    for ( int i = 0; i < game->getRobots()->size(); i++ )
    {
        cout << "Strategy::strategyHalt " << endl;
        game->getRobots()->at ( i )->setVelocity ( new MVector ( 0, 0 ) );
        game->getRobots()->at ( i )->setAngularVelocity ( 0 );
        game->getRobots()->at ( i )->execCommand();
    }
}

//ALTERAR
void Strategy::penaltyTeam ()
{

    vector < Robot * > *robots = new vector <Robot *>;
    int players = game->getPlayersOn ();

    switch ( players )
    {
        case 1:
        case 2: break;
        case 3:
            robots = gameFunctions->getRobotsDif ( this->getKicker()->getBehavior() );
            break;
        case 4:
            robots = gameFunctions->getRobotsDif ( 2, this->getKicker()->getBehavior(), DEFENDER_ONLY );
            posDefensiveGoalPost ( gameFunctions->getRobot ( DEFENDER_ONLY ) ) ;
            break;
        case 5:
            robots = gameFunctions->getRobotsDif ( 3, DEFENDER_LEFT, DEFENDER_RIGHT, this->getKicker()->getBehavior() );
            defenderWall ( gameFunctions->getRobot ( DEFENDER_LEFT ), gameFunctions->getRobot ( DEFENDER_RIGHT ) );
            break;
        case 6:
            robots = gameFunctions->getRobotsDif ( 3, DEFENDER_LEFT, DEFENDER_RIGHT, this->getKicker()->getBehavior() );
            defenderWall ( gameFunctions->getRobot ( DEFENDER_LEFT ), gameFunctions->getRobot ( DEFENDER_RIGHT ) );
    }

    double distanceLine = game->fieldSettings->robotDistancePenalty;
    game->changeSide ( distanceLine );
    distanceLine = game->getEnemyGoal()->getPointX() - distanceLine;
    Point *position;
    players = robots->size();

    int count = 1;
    for ( int i = 0; i < players; i++ )
    {
        position = new Point ( distanceLine, UtilGame::m_map ( count++, 0, players + 1, game->fieldSettings->fieldHorizontalMin, game->fieldSettings->fieldHorizontalMax ) );
        positionToTarget( robots->at ( i ),  position );
    }
    behaviorGoalkeaper();
    positionToKick ( getKicker () );
}


Robot *Strategy::getKicker ()
{
    switch ( game->getPlayersOn() )
    {
        case 1:
            return gameFunctions->getRobot ( GOALKEAPER_ONLY );
        case 2:
            return gameFunctions->getRobot ( DEFENDER_ONLY );
        case 3:
        case 4:
        {
            if ( gameFunctions->ballUpperField() )
                return gameFunctions->getRobot ( MIDFIELD_LEFT );
            else
                return gameFunctions->getRobot ( MIDFIELD_RIGHT );
        }
        case 5:
            return gameFunctions->getRobot ( MIDFIELD_ONLY );
        case 6:
            return gameFunctions->getRobot ( MIDFIELD_ONLY );
    }
}

Robot *Strategy::getPartner()
{
    switch ( game->getPlayersOn() )
    {
        case 3:
        case 4:
        {
            if ( gameFunctions->ballUpperField() )
                return gameFunctions->getRobot ( MIDFIELD_RIGHT);
            else
                return gameFunctions->getRobot ( MIDFIELD_LEFT  );
        }
        case 5:
            return gameFunctions->getRobot ( STRIKER_ONLY );
        case 6:
        {
            if ( gameFunctions->ballUpperField() )
                return gameFunctions->getRobot ( STRIKER_LEFT );
            else
                return gameFunctions->getRobot ( STRIKER_RIGHT );
        }
        default:
            return NULL;
    }
}

void Strategy::behaviorGoalkeaper ()
{
    Robot *goalKeaper = gameFunctions->getGoalKeaper();
    double x = game->getBall()->getPosition()->getPointX();
    game->changeSide ( x );

    if ( x < game->fieldSettings->fieldHorizontalMin )
    {
        positionToTarget( goalKeaper,  game->getOurGoal() );
        return;
    }

    if ( gameFunctions->ballInsideGoalArea() )
    {
        if ( game->getCmdReferee() != Stop )
            kickEnemyGoalOrPass( goalKeaper );
        else
            behaviorGoalkeaperPenalty ();
    }
    else
    {
        double dist = game->getBall()->getPosition()->getDistanceTo( game->getOurGoal() );
        double angleBall = game->getBall()->getVelocity()->getAngle();
        double valueY = game->getBall()->getPosition()->getPointY()  + dist * sin ( angleBall );
        gameFunctions->changeSideAngle ( angleBall );

        Point *position;
        if ( abs ( valueY ) <= ( game->fieldSettings->goalkeeperAreaRadius + bss->robotRadiusError ) / 2 &&
             game->getBall()->getVelocity()->getModule() > 0
             && abs ( angleBall ) > PI / 2/*sentido da bola em direção ao gol adversário*/ )
        {
            if ( game->getBall()->getPosition()->getPointZ() > game->getBall()->ballSettings->ballZ_Simulation )
                dist = 0;
            else
                dist = bss->distanceGoalMin;

            double oppAngle = game->getBall()->getVelocity()->getOppositeAngle();
            double rectX = game->getOurGoal()->getPointX() + dist * cos ( oppAngle );
            double rectY = valueY + dist * sin ( oppAngle );
            position = new Point ( rectX, rectY );
            positionToTarget( goalKeaper,  position );
        }
        else
        {
            double angle = game->getOurGoal()->calculateAngleTo ( game->getBall()->getPosition() );
            position = new Point (    game->getOurGoal()->getPointX() + bss->distanceGoalMin * cos ( angle ),
                                    game->getOurGoal()->getPointY() + bss->distanceGoalMin * sin ( angle ) );
            positionToTarget( goalKeaper,  position );
        }
    }
//    if ( behavior == GOALKEAPER_ONLY )
//        behaviorGoalkeaperOnly();
}

void Strategy::positionToKick ( Robot *robot )
{
    double angle = game->getBall()->getPosition()->calculateAngleTo ( game->getEnemyGoal() );
    Point *position = new Point ( game->getBall()->getPosition()->getPointX() - bss->robotRadiusError * cos ( angle ),
                         game->getBall()->getPosition()->getPointY() - bss->robotRadiusError * sin ( angle ) );
    positionToEnemyGoal ( robot, position );
}

void Strategy::positionToKickToTarget ( Robot *robot )
{
    double angle = game->getBall()->getPosition()->calculateAngleTo ( robot->getTarget() );
    Point *position = new Point (   game->getBall()->getPosition()->getPointX() - bss->robotRadiusError * cos ( angle ),
                                    game->getBall()->getPosition()->getPointY() - bss->robotRadiusError * sin ( angle ) );
    this->positionToTarget ( robot, position );
}

void Strategy::penaltyEnemy ()
{
    vector < Robot * > *robots = new vector <Robot *>;
    int players = game->getPlayersOn ();

    switch ( players )
    {
        case 2:
        case 3:
        case 4:
            robots = gameFunctions->getRobotsDif();
            break;
        case 5:
            robots = gameFunctions->getRobotsDif ( STRIKER_ONLY );
            auxDirectKick3 ( gameFunctions->getRobot( STRIKER_ONLY ) );
            break;
        case 6:
            robots = gameFunctions->getRobotsDif ( 2, STRIKER_LEFT, STRIKER_RIGHT );
            positionStriker2Pl ( gameFunctions->getRobot( STRIKER_LEFT ), gameFunctions->getRobot( STRIKER_RIGHT ) );
    }

    double distanceLine = game->fieldSettings->robotDistancePenalty;
    game->changeSide ( distanceLine );
    distanceLine += game->getOurGoal()->getPointX();
    Point *position;
    players = robots->size();
    int count = 1;

    for ( int i = 0; i < players; i++ )
    {
        position = new Point ( distanceLine, UtilGame::m_map ( count++, 0, players + 1, game->fieldSettings->fieldHorizontalMin, game->fieldSettings->fieldHorizontalMax ) );
        positionToTarget( robots->at ( i ),  position );
    }
    behaviorGoalkeaperPenalty ();
}

void Strategy::behaviorGoalkeaperPenalty ()
{
    Robot *goalKeaper = gameFunctions->getGoalKeaper();
    Robot *kicker = getEnemyKicker();
    double x = game->fieldSettings->goalkeeperPenalty;
    cout << "kicker = " << kicker << endl;
    if ( kicker != NULL )
    {
        double y = game->fieldSettings->penaltyMark * sin ( kicker->getOrientation() );
        game->changeSide( x );
        positionToTarget ( goalKeaper, new Point ( x, y ) );
    }
    else
    {
        game->changeSide( x );
        positionToTarget ( goalKeaper, new Point ( x, 0 ) );
    }
}

Robot *Strategy::getEnemyKicker ()
{
    vector < Robot * > *robots = game->getEnemyRobots ();
    Robot *kicker = NULL;

    for ( int i = 0; i < robots->size(); i++ )
        if ( robots->at ( i )->getPosition()->getDistanceTo ( game->getBall()->getPosition() ) <= bss->kickerDistance )
            kicker = robots->at ( i );
    return kicker;
}

bool Strategy::ballDangerousRegion ()
{
    double x = game->getBall()->getPosition()->getPointX();
    game->changeSide ( x );
    if ( x < bss->dangerousRegion1 )
        return true;
    return false;
}

//estratégia para o comando stop
void Strategy::strategyStop ()
{
    behaviorGoalkeaper ();

//    if ( gameFunctions->ballInsideGoalArea() ) //talvez mudar para a distancia do raio do gol, dist da bola e diametro do robo
//    {
//        strategyHalt();
//        return;
//    }

    switch ( game->getPlayersOn() )
    {
        case 2:
            stop2Pl();
            break;
        case 3:
            stop3Pl();
            break;
        case 4:
            stop4Pl();
            break;
        case 5:
            stop5Pl();
            break;
        case 6:
            stop6Pl();
            break;
    }
}

//faz o único robô na defesa se posicionar na reta entre a bola e o centro do gol
void Strategy::stop2Pl()
{
   auxStop1 ( gameFunctions->getRobot( DEFENDER_ONLY ), bss->distanceToBall );
}

//faz o robô com o "behavior1" se posicione sobre reta entre a bola e o centro do gol
void Strategy::auxStop1 ( Robot *robot, int distanceForBall )
{
    //double angle = angleGoalToBall();
    double angle = game->getOurGoal()->calculateAngleTo ( game->getBall()->getPosition() );
    Point *position = new Point (     game->getBall()->getPosition()->getPointX() - distanceForBall * cos ( angle ),
                                    game->getBall()->getPosition()->getPointY() - distanceForBall * sin ( angle ) );
    positionToTarget ( robot,  position );
}

//faz com que o robô com a posição y igual ao da bola execute o comportamento de auxStop1 e o outro meio de campo
//se posicione adjacente ao outro robô
void Strategy::stop3Pl()
{
    auxStop2 ( gameFunctions->getRobot ( MIDFIELD_LEFT ), gameFunctions->getRobot ( MIDFIELD_RIGHT ), bss->distanceToBall );
    //posDefGoalPostStop ( MIDFIELD_LEFT, MIDFIELD_RIGHT );
}

void Strategy::posDefGoalPostStop ( Robot *robot1, Robot *robot2)
{
    double angle = game->getOutGoalPostUpper()->calculateAngleTo ( game->getBall()->getPosition() );
    Point *position = new Point ( game->getBall()->getPosition()->getPointX() - bss->distanceToBall * cos ( angle - bss->distToBallAngle ),
                                game->getBall()->getPosition()->getPointY() - bss->distanceToBall * sin ( angle - bss->distToBallAngle ) );
    positionToTarget ( robot1, position );

    angle = game->getOutGoalPostBottom()->calculateAngleTo ( game->getBall()->getPosition() );
    position = new Point (    game->getBall()->getPosition()->getPointX() - bss->distanceToBall * cos ( angle + bss->distToBallAngle ),
                            game->getBall()->getPosition()->getPointY() - bss->distanceToBall * sin ( angle + bss->distToBallAngle ) );
    positionToTarget ( robot2, position );
}



void Strategy::auxStop2 ( Robot *robot1, Robot *robot2, int distanceToBall )
{
    double angle = game->getOurGoal()->calculateAngleTo ( game->getBall()->getPosition() );
    double angleDist = gameFunctions->angleToDist ( distanceToBall );
    Point *position;

    if ( gameFunctions->ballUpperField() )
    {
        auxStop1 ( robot2, distanceToBall );
        position = new Point (  game->getBall()->getPosition()->getPointX() - distanceToBall * cos ( angle - angleDist ),
                                game->getBall()->getPosition()->getPointY() - distanceToBall * sin ( angle - angleDist ) );
        positionToTarget( robot1,  position );
    }
    else
    {
        auxStop1 ( robot1, distanceToBall );
        position = new Point (  game->getBall()->getPosition()->getPointX() - distanceToBall * cos ( angle + angleDist ),
                                game->getBall()->getPosition()->getPointY() - distanceToBall * sin ( angle + angleDist ) );
        positionToTarget( robot2,  position );
    }
}

//posiciona um robô sobre a reta entre o gol e a bola e os outros dois robôs de meio de campo
//adjacentes ao robô  sobre a reta
void Strategy::stop4Pl()
{
    auxStop3 ( gameFunctions->getRobot ( DEFENDER_ONLY ),
               gameFunctions->getRobot ( MIDFIELD_LEFT ),
               gameFunctions->getRobot ( MIDFIELD_RIGHT ) );
}

void Strategy::auxStop3( Robot *robot1, Robot *robot2, Robot *robot3 )
{
    vector <Robot *> *enemies = getDangerousEnemies ();
    if ( enemies->size() > 0 )
    {
        markEnemy( enemies->at ( 0 ), robot1 );
        auxStop2 ( robot2, robot3, bss->distanceToBall );
    }
    else
    {
        //comentado para teste de novo comportamento
//        if ( game->getBall()->getPosition()->getDistanceTo ( game->getOurGoal() ) <= bss->distanceMinStop )
//            NDefense ( 3,   robot2, robot3, robot1 );
//        else
//        {
//            posDefensiveGoalPost ( robot1 );
//            auxStop2 ( robot2, robot3, bss->distanceToBall );
//        }
        NDefense ( 3,   robot2, robot3, robot1 );
    }
}

//comportamento dos 4 jogadores em campo, com exceção do goleiro, para o referee Stop
void Strategy::stop5Pl()
{
    Behavior behavior1, behavior2;
    if ( game->getBall()->getPosition()->getDistanceTo ( game->getOurGoal() ) <= bss->distanceMinStop )
    {
        if ( gameFunctions->ballUpperField() )
        {
            behavior1 = DEFENDER_LEFT;
            behavior2 = DEFENDER_RIGHT;
        }
        else
        {
            behavior1 = DEFENDER_RIGHT;
            behavior2 = DEFENDER_LEFT;
        }

        auxStop3 ( gameFunctions->getRobot ( behavior1 ), gameFunctions->getRobot ( MIDFIELD_ONLY ),
                   gameFunctions->getRobot ( behavior2 ) );
        auxDirectKick3 ( gameFunctions->getRobot ( STRIKER_ONLY ) );

    }
    else
    {

        if ( gameFunctions->ballUpperField() )
        {
            behavior1 = DEFENDER_LEFT;
            behavior2 = DEFENDER_RIGHT;
            NDefense ( 3,   gameFunctions->getRobot ( MIDFIELD_ONLY ),
                            gameFunctions->getRobot ( STRIKER_ONLY ), gameFunctions->getRobot ( behavior1 ) );
        }
        else
        {
            behavior1 = DEFENDER_RIGHT;
            behavior2 = DEFENDER_LEFT;
            NDefense ( 3,   gameFunctions->getRobot ( STRIKER_ONLY ),
                            gameFunctions->getRobot ( behavior1 ), gameFunctions->getRobot ( MIDFIELD_ONLY ) );

        }

        vector <Robot *> *enemies = getDangerousEnemies ();
        if ( enemies->size() > 0 )
        {
            markEnemy( enemies->at ( 0 ), gameFunctions->getRobot ( behavior2 ) );
        }
        else
        {
            posDefensiveGoalPost ( gameFunctions->getRobot(behavior2));
        }
    }
}

//comportamento dos 5 jogadores em campo, com exceção do goleiro, para o referee Stop
void Strategy::stop6Pl()
{

    Behavior behavior1 = DEFENDER_LEFT;
    Behavior behavior2 = DEFENDER_RIGHT;
    double angle = game->getOurGoal()->calculateAngleTo( game->getBall()->getPosition() );
    gameFunctions->oppositeAngle ( angle );

    if ( game->getBall()->getPosition()->getDistanceTo ( game->getOurGoal() ) <= bss->distanceMinStop ||
         abs ( angle * 180 / PI ) > 80 )
    {
        vector <Robot *> *enemies = getDangerousEnemies ();
        if ( enemies->size() >= 2 )
        {

            Robot *robot = gameFunctions->getRobotNearTo ( enemies->at ( 0 )->getPosition(), 2, gameFunctions->getRobot( behavior1 ),
                                                  gameFunctions->getRobot( behavior2  ));
            markEnemy( enemies->at ( 0 ), robot );
            if ( robot->getBehavior() == behavior1 )
                robot = gameFunctions->getRobot ( behavior2 );
            else
                robot = gameFunctions->getRobot ( behavior1 );
            markEnemy( enemies->at ( 1 ), robot );
            NDefense ( 3,   gameFunctions->getRobot ( MIDFIELD_ONLY ),
                            gameFunctions->getRobot ( STRIKER_LEFT ), gameFunctions->getRobot ( STRIKER_RIGHT ));
        }
        else if ( enemies->size() == 1 )
        {
            Robot *robot = gameFunctions->getRobotNearTo ( enemies->at ( 0 )->getPosition(), 2, gameFunctions->getRobot( behavior1 ),
                                                  gameFunctions->getRobot( behavior2  ));
            markEnemy( enemies->at ( 0 ), robot );
            if ( robot->getBehavior() == behavior1 )
                robot = gameFunctions->getRobot ( behavior2 );
            else
                robot = gameFunctions->getRobot ( behavior1 );

            NDefense ( 4,   gameFunctions->getRobot ( MIDFIELD_ONLY ),
                            gameFunctions->getRobot ( STRIKER_LEFT ),
                            gameFunctions->getRobot ( STRIKER_RIGHT ),
                            robot );
        }
        else
        {
            gameFunctions->changeBehaviors ( behavior1, behavior2 );
            NDefense ( 5,   gameFunctions->getRobot ( MIDFIELD_ONLY ),
                            gameFunctions->getRobot ( STRIKER_LEFT ),
                            gameFunctions->getRobot ( STRIKER_RIGHT ),
                            gameFunctions->getRobot ( behavior2 ),
                            gameFunctions->getRobot ( behavior1 ) );
        }
    }
    else
    {
        NDefense ( 3,   gameFunctions->getRobot ( MIDFIELD_ONLY ),
                        gameFunctions->getRobot ( STRIKER_LEFT ), gameFunctions->getRobot ( STRIKER_RIGHT ));
        auxStopGoal2Pl ( gameFunctions->getRobot( behavior1 ), gameFunctions->getRobot( behavior2 ) );

    }

}

//posiciona uma barreira com "size" jogadores, podendo size variar de 3 a 5
void Strategy::NDefense(int size, ...)
{
    Point *goal;

    double angle2;
    double angle3;
    if ( gameFunctions->ballUpperField() )
    {
        goal = game->getOutGoalPostUpper();
        angle2 = bss->distToBallAngle3Pl;
        angle3 = bss->distToBallAngle4Pl;
    }
    else
    {
        goal = game->getOutGoalPostBottom();
        angle2 = -bss->distToBallAngle3Pl;
        angle3 = -bss->distToBallAngle4Pl;
    }
    double angle = goal->calculateAngleTo ( game->getBall()->getPosition() );

    Robot *robot;
    va_list vl;
    va_start ( vl, size );
    robot = va_arg ( vl, Robot * );

    Point *position = new Point (     game->getBall()->getPosition()->getPointX() - bss->distanceToBall * cos ( angle ),
                                    game->getBall()->getPosition()->getPointY() - bss->distanceToBall * sin ( angle ) );

    positionToTarget( robot,  position );
    robot = va_arg ( vl, Robot * );

    position = new Point (    game->getBall()->getPosition()->getPointX() - bss->distanceToBall * cos ( angle + bss->distToBallAngle2Pl ),
                            game->getBall()->getPosition()->getPointY() - bss->distanceToBall * sin ( angle + bss->distToBallAngle2Pl ) );
    positionToTarget( robot,  position );
    robot = va_arg ( vl, Robot * );

    position = new Point (    game->getBall()->getPosition()->getPointX() - bss->distanceToBall * cos ( angle - bss->distToBallAngle2Pl ),
                            game->getBall()->getPosition()->getPointY() - bss->distanceToBall * sin ( angle - bss->distToBallAngle2Pl ) );
    positionToTarget( robot,  position );
    robot = va_arg ( vl, Robot * );

    if ( size > 3 )
    {
        position = new Point (    game->getBall()->getPosition()->getPointX() - bss->distanceToBall * cos ( angle + angle2 ),
                                game->getBall()->getPosition()->getPointY() - bss->distanceToBall * sin ( angle +angle2 ) );
        positionToTarget( robot,  position );
        robot = va_arg ( vl, Robot * );

        if ( size > 4 )
        {
            position = new Point (  game->getBall()->getPosition()->getPointX() - bss->distanceToBall * cos ( angle + angle3 ),
                                    game->getBall()->getPosition()->getPointY() - bss->distanceToBall * sin ( angle + angle3 ) );
            positionToTarget( robot,  position );
        }
    }
    va_end(vl);
}

void Strategy::auxStopGoal2Pl ( Robot *robot1, Robot *robot2 )
{
    Point *pUpper = game->getOutGoalPostUpper();
    Point *pBotton = game->getOutGoalPostBottom();
    double angle;
    angle = pUpper->calculateAngleTo( game->getBall()->getPosition() );
    double dist = auxDistanceToBall ( true );
    Point *position = new Point ( pUpper->getPointX() + dist * cos ( angle ),
                             pUpper->getPointY() + dist * sin ( angle ) );
    positionToTarget( robot2, position );

    angle = pBotton->calculateAngleTo( game->getBall()->getPosition() );
    dist = auxDistanceToBall ( false );
    position = new Point (pBotton->getPointX() + dist * cos ( angle ),
                        pBotton->getPointY() + dist * sin ( angle ) );
    positionToTarget( robot1, position );
}

void Strategy::posDefensiveGoalPost(Robot *robot)
{
    Point *position;
    double angle;
    double dist;

    if ( gameFunctions->ballUpperField() )
    {
        Point *pBotton = game->getOutGoalPostBottom();
        angle = pBotton->calculateAngleTo( game->getBall()->getPosition() );
        dist = auxDistanceToBall ( false );
            position = new Point (pBotton->getPointX() + dist * cos ( angle ),
                                pBotton->getPointY() + dist * sin ( angle ) );
    }
    else
    {
        Point *pUpper = game->getOutGoalPostUpper();
        angle = pUpper->calculateAngleTo( game->getBall()->getPosition() );
        dist = auxDistanceToBall ( true );
        position = new Point (pUpper->getPointX() + dist * cos ( angle ),
                            pUpper->getPointY() + dist * sin ( angle ) );
    }
    positionToTarget( robot, position );
}

//cálculo da distância para o posicionamento de um robô sobre a reta da trave superior ou inferior utilizando a lei dos cossenos
double Strategy::auxDistanceToBall ( bool is_pUpper )
{
    double angle;
    if ( is_pUpper )
        angle = game->getOutGoalPostUpper()->calculateAngleTo( game->getBall()->getPosition() );
    else
        angle = game->getOutGoalPostBottom()->calculateAngleTo( game->getBall()->getPosition() );
    gameFunctions->oppositeAngle ( angle );
    double dist;
    double a = game->fieldSettings->goalWidth / 2;
    double b = bss->distMinToOurGoal + ROBOT_RADIUS;// "+ ROBOT_RADIUS" = gambiarra para não entrar na área do goleiro

    if ( is_pUpper )
    {
        if ( angle < 0 )
            angle = PI / 2 - abs ( angle );
        else
            angle = PI / 2 + abs ( angle );
    }
    else
    {
        if ( angle < 0 )
            angle = PI / 2 + abs ( angle );
        else
            angle = PI / 2 - abs ( angle );
    }
    double k = -2 * a * cos ( angle );
    double w = -b * b + a * a;
    dist = ( -k + sqrt ( k * k - 4 * w ) ) / 2;//baskara
    return dist;
}

void Strategy::auxStopGoal ( Robot *robot )
{
    double angle = game->getOurGoal()->calculateAngleTo ( game->getBall()->getPosition() );
    Point *position = new Point (     game->getOurGoal()->getPointX() + bss->distMinToOurGoal* cos ( angle ),
                                    game->getOurGoal()->getPointY() + bss->distMinToOurGoal* sin ( angle ) );
    positionToTarget( robot,  position );
}

void Strategy::markEnemy ( Robot *enemy, Robot *marker )
{
    double angle = enemy->getPosition()->calculateAngleTo ( game->getBall()->getPosition() );
    positionToTarget( marker,
                new Point ( enemy->getPosition()->getPointX() + 2 * bss->robotRadiusError * cos ( angle ),
                            enemy->getPosition()->getPointY() + 2 * bss->robotRadiusError * sin ( angle ) ) );
    enemy->addMark ( marker );
}

vector <Robot *> *Strategy::getDangerousEnemies ()
{
    vector <Robot *> *robots = game->getEnemyRobots();
    vector <Robot *> *enemies = new vector <Robot *> ;

    for ( int i = 0; i < robots->size(); i++ )
    {
        double x = robots->at ( i )->getPosition()->getPointX();
        game->changeSide( x );
        if (  x < bss->dangerousRegion1 )
        {
            if ( robots->at ( i )->getPosition ()->getDistanceTo ( game->getBall()->getPosition() ) > bss->distanceToBallEnemy &&
                 !gameFunctions->pointInsideGoalArea ( robots->at ( i )->getPosition () ) )
                enemies->push_back ( robots->at ( i ) );
        }
    }
    return enemies;
}

//comportamento para o GOALKEAPER_ONLY
void Strategy::behaviorGoalkeaperOnly()
{
    vector < Robot * > *enemies = gameFunctions->getRobotsNearBall( game->getEnemiesOn() );
    Robot *goalkeaper = gameFunctions->getRobot( GOALKEAPER_ONLY );
    if ( enemies->size() > 0 )
    {

        double dist_gk = goalkeaper->getPosition()->getDistanceTo( game->getBall()->getPosition() );
        double dist_en = enemies->at ( 0 )->getPosition()->getDistanceTo( game->getBall()->getPosition() );
        if (  dist_gk < dist_en /*|| dist_gk <= DISTANCE_GKO_MIN*/ )
            kickEnemyGoalOrPass( goalkeaper );
    }
    else
    {
        kickEnemyGoalOrPass( goalkeaper );
    }
}

//calcula a posição do robô para que o mesmo se pocione em direção ao gol adversário
void Strategy::kickEnemyGoalOrPass ( Robot *robot )
{
    //Robot *robot = gameFunctions->getRobot ( behavior );
    positionToKick ( robot );
    Point *target;

    if ( gameFunctions->ballInsideEnemyGoalArea() ) //NAO FOI TESTADO PARA O TIME AMARELO!!!!!!!!!!!
    {
        robot->setTarget ( game->getEnemyGoal()  );
        positionToKick ( robot );

        double angleToBall = robot->calculateAngleTo( game->getBall()->getPosition() );
        gameFunctions->oppositeAngle( angleToBall );
        double angleToEnGoal = robot->calculateAngleTo( game->getEnemyGoal() );
        gameFunctions->oppositeAngle( angleToEnGoal );
        if ( abs ( angleToBall ) * 180 / PI <  bss->forceKickAngle &&
             abs ( angleToEnGoal ) * 180 / PI <  bss->forceKickAngle )
        {
            this->makeRobotShot( robot );
           this->goToPos( robot, game->getEnemyGoal(), robot->getTarget() );

        }
    }
    else if ( robot->getPosition()->getDistanceTo ( robot->getPosToGo() ) < ROBOT_DIAMETER )
    {
        vector <Robot *> *partners = this->getPartners ( robot );
        int size = partners->size();
        if ( this->enemyOnTheLine ( game->getBall()->getPosition(), game->getEnemyGoal() ) > 0 && size > 0 )
        {
            Robot *partner = NULL;
            double distToGoal = -1;
            for ( int i = 0; i < size; i++ )
            {
                if ( this->enemyOnTheLine ( game->getBall()->getPosition(), partners->at ( i )->getPosition() ) < 0 &&
                     partners->at ( i )->isReady() && !gameFunctions->pointIsOnOurSide ( partners->at ( i )->getPosition() ) )
                {
                    if ( distToGoal == -1 )
                    {
                        distToGoal = partners->at ( i )->getPosition()->getDistanceTo ( game->getEnemyGoal() );
                        partner = partners->at ( i );
                    }
                    else
                    {
                        double dist = partners->at ( i )->getPosition()->getDistanceTo ( game->getEnemyGoal() );
                        if ( dist < distToGoal )
                        {
                            distToGoal = dist;
                            partner = partners->at ( i );
                        }
                    }

                }
            }

//            cout << "PARTNER = "  << partner << endl;
            if ( partner != NULL )
            {
                positionToPass ( robot, partner );
                goToBallKicking( robot, partner->getPosition() );
            }
            else
            {
                target = game->getEnemyGoal();
                goToBallKicking( robot, target );
            }

        }
        else
        {
            target = game->getEnemyGoal();
            goToBallKicking( robot, target );
        }
    }
}

//se locomove em direção ao ponto position direcionado para o alvo anteriormente definido
void Strategy::positionToTarget ( Robot *robot, Point *position )
{
    goToPos ( robot, position, game->getBall()->getPosition() );
}

//vai para o ponto position mantendo-se direcionado para o gol adversário
void Strategy::positionToEnemyGoal ( Robot *robot, Point *position )
{
    goToPos ( robot, position, game->getEnemyGoal() );
}

void Strategy::goToPos(Robot *robot, Point *position, Point *target)
{
    robot->setPosToGo( position );
    robot->setTarget ( target );
    robot->calculateDistances ();
    robot->calculateVelocities();
    robot->execCommand();
}

void Strategy::goToValPos ( )
{
    if ( !gameFunctions->ballInsideGoalArea() )
        kickEnemyGoalOrPass ( gameFunctions->getRobotNearToBall() );
}

void Strategy::goToBallKicking(Robot *robot, Point *target)
{
    if ( robot->isReady() )
    {
        robot->setTarget ( target );
        this->makeRobotShot( robot );
        positionToTarget ( robot, game->getBall()->getPosition() );
    }
}

//retorna a distância para o inimigo mais próximo do ponto inicial, bPoint, até o ponto final, fPoint
double Strategy::enemyOnTheLine( Point *bPoint, Point *fPoint )
{
    vector < Robot * > *enemies = game->getEnemyRobots();
    double angle = bPoint->calculateAngleTo ( fPoint );
    Point *nextPoint = bPoint;
    while ( nextPoint->getDistanceTo ( fPoint ) > ROBOT_RADIUS )
    {
        for ( int i = 0; i < enemies->size(); i++ )
        {
            //if ( enemies->at ( i )->getPosition()->getDistanceTo ( nextPoint ) <= ROBOT_DIAMETER )
            //com o ROBOT_DIAMETER, o "if" avalia melhor, porem, o robô próximo à trave oposta fica sempre como marcado, então
            //que desfavore a estratégia de passe
            if ( enemies->at ( i )->getPosition()->getDistanceTo ( nextPoint ) <= ROBOT_RADIUS )
            {
                return bPoint->getDistanceTo ( enemies->at ( i )->getPosition() );
            }
        }
        nextPoint = new Point ( nextPoint->getPointX() + ROBOT_RADIUS * cos ( angle ),
                                nextPoint->getPointY() + ROBOT_RADIUS * sin ( angle ) );
    }
    return -1;
}

//retorna os outros robôs que atuam como atacantes ou meio de campo
vector<Robot *> *Strategy::getPartners ( Robot *robot )
{
    vector < Robot * > *partners = new vector < Robot * >;
    switch ( game->getPlayersOn() )
    {
        case 3:
            partners = gameFunctions->getRobotsDif ( robot->getBehavior() );
            break;
        case 4:
            partners = gameFunctions->getRobotsDif ( 2, robot->getBehavior(), DEFENDER_ONLY );
            break;
        case 5:
        case 6:
            partners = gameFunctions->getRobotsDif ( 3, robot->getBehavior(), DEFENDER_LEFT, DEFENDER_RIGHT );
    }
    return partners;
}

void Strategy::makeRobotShot (Robot *robot)
{
    robot->setGoingToBall ( true );
    if ( this->enemyOnTheLine ( game->getBall()->getPosition(), robot->getTarget() ) > 0 )
    {
        robot->calculateKickWithZ();
        robot->calculateKickZ();
    }
    else
        robot->calculateKick();

}

void Strategy::makeRobotPass(Robot *robot)
{
    robot->setGoingToBall ( true );
    if ( this->enemyOnTheLine ( game->getBall()->getPosition(), robot->getTarget() ) > 0 )
    {
        robot->calculateKickWithZPartner();
        robot->calculateKickZPartner();
    }
    else
        robot->calculateKickToPartner();
}
