#include "Game.h"

GameFunctions::GameFunctions ( Game *_game )
{
    game = _game;
}

vector<Robot *> *GameFunctions::getRobotsNearBall( vector<Robot *> *m_robots )
{
    Robot **ord_robots = new Robot *[ m_robots->size() ];
    for ( int i = 0; i < m_robots->size(); i++ )
        ord_robots [i] = m_robots->at(i);

    if ( m_robots->size() > 1 )
    {
        int tam = m_robots->size();
        for ( int i = 0; i < (tam-1); i++)
        {
            int min = i;
            for ( int j = (i+1); j < tam; j++)
            {
                if( ord_robots[j]->getPosition()->getDistanceTo( game->getBall()->getPosition() ) <
                        ord_robots[min]->getPosition()->getDistanceTo( game->getBall()->getPosition() ) )
                {
                    min = j;
                }
            }
            if (i != min)
            {
                Robot *swap = ord_robots[i];
                ord_robots[i] = ord_robots[min];
                ord_robots[min] = swap;
            }
        }
        m_robots->clear();
        for ( int i = 0; i < tam; i++ )
            m_robots->push_back ( ord_robots [i] );

    }

    return m_robots;
}

vector<Robot *> *GameFunctions::getRobotsDif ( vector <Behavior> *behavior )
{
    vector<Robot *> *robots = game->getRobotsOn();
    vector<Robot *> *robotsDif = new vector<Robot *> ;

    for ( int i = 0; i < robots->size(); i++ )
    {
        if ( robots->at ( i )->getBehavior() == GOALKEAPER ||
             robots->at ( i )->getBehavior() == GOALKEAPER_ONLY )
            continue;

        bool insert = true;
        for ( int j = 0; j < behavior->size(); j++ )
        {
            if ( robots->at ( i )->getBehavior() == behavior->at ( j ) )
                insert = false;
        }
        if ( insert )
            robotsDif->push_back ( robots->at ( i ) );
    }
    return robotsDif;
}

vector<Robot *> *GameFunctions::getRobotsDif ( Behavior behavior )
{
    vector<Robot *> *robots = game->getRobotsOn();
    vector<Robot *> *robotsDif = new vector<Robot *> ;

    for ( int i = 0; i < robots->size(); i++ )
    {
        if ( robots->at ( i )->getBehavior() == GOALKEAPER ||
             robots->at ( i )->getBehavior() == GOALKEAPER_ONLY ||
             robots->at ( i )->getBehavior() == behavior )
            continue;
        robotsDif->push_back ( robots->at ( i ) );
    }
    return robotsDif;
}

vector<Robot *> *GameFunctions::getRobotsDif( int size, ... )
{
    vector<Robot *> *robots = game->getRobotsOn();
    vector<Robot *> *robotsDif = new vector<Robot *> ;

    int behavior;
    for ( int i = 0; i < robots->size(); i++ )
    {
        if ( robots->at ( i )->getBehavior() == GOALKEAPER ||
             robots->at ( i )->getBehavior() == GOALKEAPER_ONLY )
            continue;
        va_list vl;
        va_start ( vl, size );
        bool insert = true;
        for ( int j = 0; j < size; j++ )
        {
            behavior = va_arg ( vl, int );
            if ( robots->at ( i )->getBehavior() == ((Behavior)behavior) )
                insert = false;
        }
        if ( insert )
            robotsDif->push_back ( robots->at ( i ) );
        va_end(vl);
    }

    return robotsDif;
}

vector<Robot *> *GameFunctions::getRobotsDifWithGoalk ( int size, ... )
{
    vector<Robot *> *robots = game->getRobotsOn();
    vector<Robot *> *robotsDif = new vector<Robot *> ;

    int behavior;
    for ( int i = 0; i < robots->size(); i++ )
    {
        va_list vl;
        va_start ( vl, size );
        bool insert = true;
        for ( int j = 0; j < size; j++ )
        {
            behavior = va_arg ( vl, int );
            if ( robots->at ( i )->getBehavior() == ((Behavior)behavior) )
                insert = false;
        }
        if ( insert )
            robotsDif->push_back ( robots->at ( i ) );
        va_end(vl);
    }

    return robotsDif;
}

vector<Robot *> *GameFunctions::getRobotsDif()
{
    vector<Robot *> *robots = game->getRobotsOn();
    vector<Robot *> *robotsDif = new vector<Robot *> ;

    for ( int i = 0; i < robots->size(); i++ )
    {
        if ( robots->at ( i )->getBehavior() == GOALKEAPER ||
             robots->at ( i )->getBehavior() == GOALKEAPER_ONLY )
            continue;
        robotsDif->push_back ( robots->at ( i ) );
    }
    return robotsDif;
}

Point *GameFunctions::ballNearRobot ( Robot*kicker, Robot **nRobot )
{
    Point *nextPoint = game->getBall()->getPosition();
    double angle = game->getBall()->getVelocity()->getAngle();

    nextPoint->setPointX ( nextPoint->getPointX() + ROBOT_DIAMETER * cos ( angle ) );
    nextPoint->setPointY ( nextPoint->getPointY() + ROBOT_DIAMETER * sin ( angle ) );


    Robot *robot = this->getRobotNearTo( nextPoint, this->getRobotsDif ( kicker->getBehavior() )  );
    double dist = robot->getPosition()->getDistanceTo( nextPoint );

    while ( !this->isOutsideField ( nextPoint ) )
    {
        Robot *auxRobot = this->getRobotNearTo( nextPoint, this->getRobotsDif ( kicker->getBehavior() ) );
        double auxDist = auxRobot->getPosition()->getDistanceTo( nextPoint );

        if ( auxDist < dist )
        {
            dist = auxDist;
            robot = auxRobot;
        }

        nextPoint->setPointX ( nextPoint->getPointX() + ROBOT_RADIUS * cos ( angle ) );
        nextPoint->setPointY ( nextPoint->getPointY() + ROBOT_RADIUS * sin ( angle ) );
    }
    *nRobot = robot;
    return nextPoint;
}

void GameFunctions::changeBehaviors(Behavior &behavior1, Behavior &behavior2)
{
    if ( !this->ballUpperField() )
    {
        Behavior swap = behavior1;
        behavior1 = behavior2;
        behavior2 = swap;
    }
}

double GameFunctions::getObjectNear(vector<double> *distances)
{
    double dist = distances->at ( 0 );
    double size = distances->size();
    for ( int i = 1; i < size; i++ )
        if ( dist > distances->at ( i ) )
            dist = distances->at ( i );
    return dist;
}

bool GameFunctions::ballInsideGoalArea()
{
    return pointInsideGoalArea ( game->getBall()->getPosition() );
}

bool GameFunctions::pointInsideGoalArea ( Point *point )
{
    return ( game->getOurGoal()->getDistanceTo ( point ) <= game->fieldSettings->goalkeeperAreaRadius );
}

Robot *GameFunctions::getGoalKeaper()
{
    if ( game->getPlayersOn() > 1 )
        return this->getRobot ( GOALKEAPER );
    else
        return this->getRobot ( GOALKEAPER_ONLY );
}

Robot *GameFunctions::getRobotNearToBall()
{
    return this->getRobotNearTo( game->getBall()->getPosition() );
}

Robot *GameFunctions::getRobotNearTo ( Point *point )
{
    vector < Robot * > *robots = this->getRobotsDif ( );
    Robot *robot = NULL;

    if ( robots->size() > 0 )
    {
        robot = robots->at ( 0 );
        double dist = robot->getPosition()->getDistanceTo ( point );
        for ( int i = 1; i < robots->size(); i++ )
        {
            double dist2 = robots->at ( i )->getPosition()->getDistanceTo ( point );
            if ( dist2 < dist )
            {
                dist = dist2;
                robot = robots->at ( i );
            }
        }
    }
    return robot;
}

Robot *GameFunctions::getRobotNearTo(Point *point, vector<Robot *> *m_robots)
{
    vector < Robot * > *robots = m_robots;
    Robot *robot = NULL;

    if ( robots->size() > 0 )
    {
        robot = robots->at ( 0 );
        double dist = robot->getPosition()->getDistanceTo ( point );
        for ( int i = 1; i < robots->size(); i++ )
        {
            double dist2 = robots->at ( i )->getPosition()->getDistanceTo ( point );
            if ( dist2 < dist )
            {
                dist = dist2;
                robot = robots->at ( i );
            }
        }
    }
    return robot;
}

Robot *GameFunctions::getRobotNearToBall ( int size, ... )
{
    Robot *robot, *robotNear;
    va_list vl;
    va_start ( vl, size );
    robotNear = robot = va_arg ( vl, Robot * );
    double dist = game->getBall()->getPosition()->getDistanceTo ( robot->getPosition() );

    for ( int i = 0; i < size - 1; i++ )
    {
        robot = va_arg ( vl, Robot * );
        double dist2 = game->getBall()->getPosition()->getDistanceTo ( robot->getPosition() );
        if ( dist2 < dist )
        {
            robotNear = robot;
            dist = dist2;
        }
    }
    va_end(vl);
    return robotNear;
}

Robot *GameFunctions::getRobotNearTo(Point *point, int size, ... )
{
    Robot *robot, *robotNear;
    va_list vl;
    va_start ( vl, size );
    robotNear = robot = va_arg ( vl, Robot * );
    double dist = point->getDistanceTo ( robot->getPosition() );

    for ( int i = 0; i < size - 1; i++ )
    {
        robot = va_arg ( vl, Robot * );
        double dist2 = point->getDistanceTo ( robot->getPosition() );
        if ( dist2 < dist )
        {
            robotNear = robot;
            dist = dist2;
        }
    }
    va_end(vl);
    return robotNear;
}

bool GameFunctions::ballInsideEnemyGoalArea()
{
    return ( game->getBall()->getPosition()->getDistanceTo ( game->getEnemyGoal() ) <= game->fieldSettings->goalkeeperAreaRadius );
}

bool GameFunctions::foulForEnemy()
{
    char cmdReferee = game->getCmdReferee();
    if ( game->getTeamColor() == BLUE )
    {
        if ( cmdReferee == KickOffYellow ||
             cmdReferee == DirectFreeKickYellow ||
             cmdReferee == IndirectFreeKickYellow
            )
            return true;
        char lastCmdReferee = game->getLastCmdReferee();
        if ( cmdReferee == Ready )
        {
            if ( lastCmdReferee == KickOffYellow ||
                 lastCmdReferee == DirectFreeKickYellow ||
                 lastCmdReferee == IndirectFreeKickYellow
                )
                return true;
        }
    }
    else
    {
        if ( cmdReferee == KickOffBlue ||
             cmdReferee == DirectFreeKickBlue ||
             cmdReferee == IndirectFreeKickBlue
            )
                return true;
        char lastCmdReferee = game->getLastCmdReferee();
        if ( cmdReferee == Ready )
        {
            if ( lastCmdReferee == KickOffBlue ||
                 lastCmdReferee == DirectFreeKickBlue ||
                 lastCmdReferee == IndirectFreeKickBlue
                )
                    return true;
        }
    }
    return false;
}

bool GameFunctions::validatePoint(Point *point)
{
    //point está dentro da área do goleiro
    if ( point->getDistanceTo ( game->getOurGoal() ) <= game->fieldSettings->goalkeeperAreaRadius + ROBOT_RADIUS )
        return false;
    //point está a uma distância inválida da bola
    if ( foulForEnemy() && point->getDistanceTo ( game->getBall()->getPosition() ) <= game->getStrategy()->bss->distanceToBall )
        return false;
    //comando Ready do referee foi para o adversário e point está a uma distância inválida da bola
    if ( game->getReadyEnemy() && point->getDistanceTo ( game->getBall()->getPosition() ) <= game->getStrategy()->bss->distanceToBall )
        return false;
    return true;
}

double GameFunctions::oppositeAngle ( double &angle )
{
    if ( game->getSide() == RIGHT )
    {
        angle -= PI;
        adjustAngle ( angle );
    }
}

double GameFunctions::reflectedAngle ( double &angle )
{
    angle = -angle;
}

double GameFunctions::adjustAngle ( double &angle )
{
    if ( angle > PI )
        angle -= 2 * PI;
    else if ( angle < -PI )
        angle += 2 * PI;
}

bool GameFunctions::isOutsideField(Point *point)
{
    double x = point->getPointX();
    double y = point->getPointY();

    if (    x < game->fieldSettings->fieldHorizontalMin || x > game->fieldSettings->fieldHorizontalMax ||
            y < game->fieldSettings->fieldVerticalMin || y > game->fieldSettings->fieldVerticalMax )
        return true;
    return false;
}

double GameFunctions::angleToDist(double dist)
{
    double angle = atan2 ( 2.2 * ROBOT_RADIUS, dist );
    adjustAngle ( angle );
    return angle;
}

void GameFunctions::changeSideAngle ( double &angle )
{
    this->oppositeAngle ( angle );
    if ( this->ballUpperField() )
        this->reflectedAngle ( angle );
}

bool GameFunctions::ballUpperField()
{
    double y = game->getBall()->getPosition()->getPointY();
    game->changeSide ( y );
    if ( y > 0 )
        return true;
    else
        return false;
}

//retorna o robô com o comportamento especificado
Robot *GameFunctions::getRobot ( Behavior mBehavior )
{
    int teamSize = game->getRobots()->size();
    vector <Robot *> *robots = game->getRobotsOn();
    for ( int i = 0; i < teamSize; i++ )
        if ( robots->at ( i )->getBehavior() == mBehavior )
            return robots->at ( i );
    return NULL;
}

bool GameFunctions::ballIsOnOurSide()
{
    return pointIsOnOurSide ( game->getBall()->getPosition() );
}

bool GameFunctions::pointIsOnOurSide(Point *point)
{
    double x = point->getPointX();
    game->changeSide( x );
    if ( x < 0 )
        return true;
    return false;
}

bool GameFunctions::enemyOnOurSide()
{
    vector < Robot * > *enemy_robots = game->getEnemyRobots();
    int size = enemy_robots->size();
    for ( int i = 0; i < size; i++ )
        if ( this->pointIsOnOurSide ( enemy_robots->at ( i )->getPosition() ) )
            return true;
    return false;
}


