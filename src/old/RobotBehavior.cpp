#include "Strategy.h"
#include "RobotBehavior.h"
#include "Game.h"
#include <QDebug>

RobotBehavior::RobotBehavior ( Behavior _behavior, Robot *_robot, Strategy *_strategy ) :
    BEHAVIOR ( _behavior )
{
    robot = _robot;
    strategy = _strategy;
    objFlie = new flie;
    rangeHigh = objFlie->getRHigh();
}

RobotBehavior::~RobotBehavior()
{
}

void RobotBehavior::calculateVelocities()
{
    Point *pointIntercp = this->ballInterception();
    if ( pointIntercp != NULL )
    {
        robot->setPosToGo ( pointIntercp );
        robot->setTarget ( strategy->getGame()->getEnemyGoal() );
    }

//    if ( strategy->getGame()->getTeamColor() == YELLOW ) //TESTE
//    {
//        //this->mapVelocities();
//        if ( robot->getBehavior() == GOALKEAPER || robot->getBehavior() == GOALKEAPER_ONLY )
//        {
//            this->mapVelocities();
//        }
//        else
//        {
//            robot->setAngularVelocity ( 0 );
//            robot->getVelocity()->setNormalDirection ( 0 );
//            robot->getVelocity()->setTangentialDirection ( 0 );
//        }

//        return;
//    }


    if ( notCallFuzzy() )
    {

        this->mapVelocities();
    }
    else
    {
        this->fuzzyLinearVelocity();
        this->mapAngularVelocity();
    }
}

bool RobotBehavior::notCallFuzzy()
{
    double *distances = robot->getDistances();
    double distToPos = robot->getPosition()->getDistanceTo ( robot->getPosToGo() );

    return( !robot->foundObstacle() || //nao achou nenhum obstaculo
            distances [ robot->DIR_FORWARD ] < 0 || //achou algum obstaculo, mas o sensor da frente não
            distToPos < ROBOT_DIAMETER ||
            robot->getGoingToBall() ||
            robot->getVelocity()->getModule() <= MIN_VEL_FUZZY
           );
}

void RobotBehavior::fuzzyLinearVelocity()
{
    float flie_velocity, flie_direction;
    float se, sf, sd;//sensores virtuais
    double *distances = robot->getDistances();

    //o sensor frente avalia a posição para a qual o robô deve se posicionar
//    sf = distances [ robot->DIR_FORWARD ] >= 0 ? distances [ robot->DIR_FORWARD ] : robot->getPosition()->getDistanceTo ( robot->getPosToGo() ) + ROBOT_RADIUS;

//    se = distances [ robot->DIR_LEFT ] >= 0 ? distances [ robot->DIR_LEFT ] : rangeHigh;
//    sd = distances [ robot->DIR_RIGHT ] >= 0 ? distances [ robot->DIR_RIGHT ] : rangeHigh;

//    sf = distances [ robot->DIR_FORWARD ] < 0 || distances [ robot->DIR_FORWARD ] > rangeHigh ?
//                distances [ robot->DIR_FORWARD ] : robot->getPosition()->getDistanceTo ( robot->getPosToGo() ) + ROBOT_RADIUS;

    if ( distances [ robot->DIR_FORWARD ] < 0 || distances [ robot->DIR_FORWARD ] > rangeHigh ||
         ( robot->getPosition()->getDistanceTo ( robot->getPosToGo() ) + ROBOT_RADIUS ) > rangeHigh )
        sf = rangeHigh;
    else
        sf = distances [ robot->DIR_FORWARD ];

    se = distances [ robot->DIR_LEFT ] <  0 || distances [ robot->DIR_LEFT ] > rangeHigh ?  rangeHigh : distances [ robot->DIR_LEFT ];
    sd = distances [ robot->DIR_RIGHT ] <  0 || distances [ robot->DIR_RIGHT ] > rangeHigh ?  rangeHigh : distances [ robot->DIR_RIGHT ];



    objFlie->inference( se, sf, sd, flie_velocity, flie_direction );
    flie_direction -= 90;// a direcao esta definida entre 0 e 180 graus na logica fuzzy, correspondendo 90 a frente
    flie_direction *=  PI / 180;//conversão para radianos

//    cout << "flie_velocity = " << flie_velocity << "\tflie_direction = " << flie_direction * 180 / PI << endl;

//    if ( robot->getId() == 5 )
//    {
//        cout << "\nse = " << se <<
//                "\tsf = " << sf <<
//                "\tsd = " << sd << endl;
//        cout << "flie_velocity = " << flie_velocity << "\tflie_direction = " << flie_direction * 180 / PI << endl;
//    }

    double angle = robot->calculateAngleTo ( robot->getPosToGo() ) + flie_direction;

//    if ( robot->getPosition()->getDistanceTo ( robot->getPosToGo() ) < 105 )
//        flie_velocity = 0;

    robot->getVelocity()->setNormalDirection ( flie_velocity * sin ( angle ) );
    robot->getVelocity()->setTangentialDirection ( flie_velocity * cos ( angle ) );
}

void RobotBehavior::mapVelocities()
{
    mapLinearVelocity ();
    mapAngularVelocity ();
}

void RobotBehavior::mapLinearVelocity ()
{
    double angDistance = robot->calculateAngleTo ( robot->getPosToGo() );
    double velocityMod = calculateMapLinearVelocity ();
//    if ( !(robot->getId() != 5 || robot->getColor() == YELLOW) )
//        cout << "angDistance = " << angDistance * 180 / PI << endl;
    robot->getVelocity()->setTangentialDirection( velocityMod * cos ( angDistance ) );
    robot->getVelocity()->setNormalDirection( velocityMod * sin ( angDistance ) );
}

void RobotBehavior::mapAngularVelocity()
{
    double velocityAng = calculateMapAngularVelocity ();
    robot->setAngularVelocity ( velocityAng );
}

double RobotBehavior::calculateMapLinearVelocity()
{
    double velocity = 0.0;
    double distance = robot->getPosition()->getDistanceTo ( robot->getPosToGo() );
    int velocityMax = 4;

    if ( distance > 2000 )
        velocity = velocityMax;
    else if ( distance < strategy->bss->errorDistance )
        velocity = 0;
    else
        velocity = UtilGame::m_map ( distance, 0, 2000, 0.3, velocityMax );

    return velocity;
}

double RobotBehavior::calculateMapAngularVelocity()
{
    double velocityAng = 0;
    double distMin;
    double distanceAngularBall = robot->calculateAngleTo ( robot->getTarget() ) * 180 / PI;
    //distMin = bss->angleMinOurSide;

    if ( strategy->getGameFunctions()->ballIsOnOurSide() )
        distMin = strategy->bss->angleMinOurSide;
    else
        distMin = strategy->bss->angleMin;

    if ( abs ( distanceAngularBall ) > 90 )
        velocityAng = UtilGame::m_map ( abs ( distanceAngularBall ), 90, 180, 4, 8 );
    else if ( abs ( distanceAngularBall ) > 40 )
        velocityAng = UtilGame::m_map ( abs ( distanceAngularBall ), 40, 90, 2, 4 );
    else if ( abs ( distanceAngularBall ) > distMin )
        velocityAng = UtilGame::m_map ( abs ( distanceAngularBall ), distMin, 40, 1, 2 );
    return velocityAng * distanceAngularBall / abs ( distanceAngularBall );
}


double RobotBehavior::calculateMapKickZ()
{
#ifdef DONT_KICK_Z
    return 0;
#endif
    double distance = robot->getPosition()->getDistanceTo ( robot->getTarget() );
    return UtilGame::m_map ( distance, 2 * strategy->bss->robotRadiusError,
                             2 * strategy->getGame()->fieldSettings->fieldHorizontalMax, 1, 4 );
}

double RobotBehavior::calculateMapKickWithZ()
{
#ifdef DONT_KICK_Z
    return strategy->bss->kickMax;;
#endif
    double distance = robot->getPosition()->getDistanceTo ( robot->getTarget() );
    return UtilGame::m_map ( distance, 2 * strategy->bss->robotRadiusError,
                             2 * strategy->getGame()->fieldSettings->fieldHorizontalMax, 4, strategy->bss->kickMax );
}

double RobotBehavior::calculateMapKick()
{
    return strategy->bss->kickMax;
    //return 5;//teste
}

double RobotBehavior::calculateMapKickZPartner()
{
    double distance = robot->getPosition()->getDistanceTo ( robot->getTarget() );
    return UtilGame::m_map ( distance, 2 * strategy->bss->robotRadiusError,
                             2 * strategy->getGame()->fieldSettings->fieldHorizontalMax, 1, 3 );
}

double RobotBehavior::calculateMapKickWithZPartner()
{
    double distance = robot->getPosition()->getDistanceTo ( robot->getTarget() );
    return UtilGame::m_map ( distance, 2 * strategy->bss->robotRadiusError,
                             2 * strategy->getGame()->fieldSettings->fieldHorizontalMax, 2, 5 );
}

double RobotBehavior::calculateMapKickToPartner()
{
    double distance = robot->getPosition()->getDistanceTo ( robot->getTarget() );
    return UtilGame::m_map ( distance, 2 * strategy->bss->robotRadiusError,
                             2 * strategy->getGame()->fieldSettings->fieldHorizontalMax, 2, 5 );
}

Point *RobotBehavior::ballInterception()
{
    Ball *ball = strategy->getGame()->getBall();
    if ( ball->getVelocity()->getModule() > 0 )
    {
        double angleBall = ball->getVelocity()->getAngle();
        double angleRobot = robot->getPosition()->calculateAngleTo ( ball->getPosition() );
        if ( ( angleBall < 0 && angleRobot > 0 ) || ( angleBall > 0 && angleRobot < 0 ) )// bola está se aproximando da posição do robô
        {
//            if ( robot->getId() != 0 )//teste
//                cout << "vao colidir !!!!!!!!\n";
            Point *nextPoint = ball->getPosition();
            double distToRobot = robot->getPosition()->getDistanceTo ( nextPoint );

            Point *fPoint = new Point ( nextPoint->getPointX() + distToRobot * cos ( angleBall ),
                                        nextPoint->getPointY() + distToRobot * sin ( angleBall ) );

            while ( nextPoint->getDistanceTo ( fPoint ) > ROBOT_RADIUS )
            {

                double dist = nextPoint->getDistanceTo ( robot->getPosition() );

                if ( dist <= ROBOT_DIAMETER )
                {
                    return nextPoint;
                }

                nextPoint = new Point ( nextPoint->getPointX() + ROBOT_DIAMETER * cos ( angleBall ),
                                        nextPoint->getPointY() + ROBOT_DIAMETER * sin ( angleBall ) );
            }
        }
    }
    return NULL;
}

const Behavior RobotBehavior::getBehavior()
{
    return BEHAVIOR;
}

double RobotBehavior::auxCalculateRobotDistances(double angle)
{

    //vector < Robot * > *enemy_robots = strategy->getGame()->getEnemyRobots();
    //vector < Robot * > *team_robots = strategy->getGameFunctions()->getRobotsDifWithGoalk( 1, robot->getBehavior() );
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
        this->ourGoalObstacle ( distances, nextPoint );
        nextPoint = new Point ( nextPoint->getPointX() + ROBOT_RADIUS * cos ( finalAngle ),
                                nextPoint->getPointY() + ROBOT_RADIUS * sin ( finalAngle ) );
    }

    if ( distances->size() > 0 )
        return strategy->getGameFunctions()->getObjectNear ( distances );
    return -1;
}

void RobotBehavior::enemyObstacle(vector<double> *distances, Point *nextPoint)
{
    vector < Robot * > *enemy_robots = strategy->getGame()->getEnemyRobots();
    int size = enemy_robots->size();
    for ( int i = 0; i < size; i++ )
    {
        if ( enemy_robots->at ( i )->getPosition()->getDistanceTo ( nextPoint ) <= ROBOT_DIAMETER )
        {
            distances->push_back ( robot->getPosition()->getDistanceTo ( enemy_robots->at ( i )->getPosition() ) );
        }
    }
}

void RobotBehavior::teamPartnerObstacle(vector<double> *distances, Point *nextPoint)
{
    //distancia dos outros robos do nosso time
    vector < Robot * > *team_robots = strategy->getGameFunctions()->getRobotsDifWithGoalk( 1, robot->getBehavior() );
    int size = team_robots->size();
    for ( int i = 0; i < size; i++ )
    {
        if ( team_robots->at ( i )->getPosition()->getDistanceTo ( nextPoint ) <= ROBOT_DIAMETER )
        {
            distances->push_back ( robot->getPosition()->getDistanceTo ( team_robots->at ( i )->getPosition() ) );
        }
    }
}

void RobotBehavior::ballObstacle(vector<double> *distances, Point *nextPoint)
{
    double dist = nextPoint->getDistanceTo ( strategy->getGame()->getBall()->getPosition() );

    if ( !robot->getGoingToBall() )
        if ( dist <= ROBOT_DIAMETER )
        {
            distances->push_back ( robot->getPosition()->getDistanceTo ( strategy->getGame()->getBall()->getPosition() ) );
        }
}

void RobotBehavior::ourGoalObstacle(vector<double> *distances, Point *nextPoint)
{

    double dist = nextPoint->getDistanceTo ( strategy->getGame()->getOurGoal () );
    if ( dist <= strategy->getGame()->fieldSettings->goalkeeperAreaRadius )
    {
        //cout << "ID = " << robot->getId() << " RobotBehavior::ourGoalObstacle\n";
        distances->push_back ( robot->getPosition()->getDistanceTo ( strategy->getGame()->getOurGoal () ) );
    }
}

flie *RobotBehavior::getObjFLie()
{
    return objFlie;
}
