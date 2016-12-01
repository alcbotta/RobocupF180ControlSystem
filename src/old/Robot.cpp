#include "RobotBehavior.h"
#include "CommunicationSystem.h"
#include "ComSysSimulator.h"

Robot::Robot ( int _id, TeamColor *_color, int _number, CommunicationSystem *_comSystem  ) :
    ID ( _id ), NUMBER ( _number ),
    ANGLE_DIRECTION ( 40 * PI / 180 ) //graus convertido em radianos
{
    comSystem = _comSystem;
    color = _color;
    orientation = 0;
    isOn = false;
    kick = 0;
    dribble = 0;
    angularVelocity = 0;
    offTime1 = 0.0;
    isMarkedBy = new vector <Robot *> ;
    possessionBall = false;
    posToGo = new Point ();
    distances = new double [ ROBOT_DISTANCES ];
    target = NULL;

    int enumBehaviorSize = 13;


    behaviorStr = new string [ enumBehaviorSize ];
    behaviorStr [ GOALKEAPER ] = "GOALKEAPER";
    behaviorStr [ GOALKEAPER_ONLY ] = "GOALKEAPER_ONLY";
    behaviorStr [ DEFENDER_ONLY ] = "DEFENDER_ONLY";
    behaviorStr [ DEFENDER_LEFT ] = "DEFENDER_LEFT";
    behaviorStr [ DEFENDER_RIGHT ] = "DEFENDER_RIGHT";
    behaviorStr [ MIDFIELD_ONLY ] = "MIDFIELD_ONLY";
    behaviorStr [ MIDFIELD_LEFT ] = "MIDFIELD_LEFT";
    behaviorStr [ MIDFIELD_RIGHT ] = "MIDFIELD_RIGHT";
    behaviorStr [ STRIKER_ONLY ] = "STRIKER_ONLY";
    behaviorStr [ STRIKER_LEFT ] = "STRIKER_LEFT";
    behaviorStr [ STRIKER_RIGHT ] = "STRIKER_RIGHT";
    behaviorStr [ OFF ] = "OFF";
    behaviorStr [ ENEMY ] = "ENEMY";
}

TeamColor Robot::getColor()
{
    return *color;
}

const int Robot::getNumber()
{
    return NUMBER;
}

void Robot::setOrientation ( double newOrientation )
{
    orientation = newOrientation;
}

double Robot::getOrientation ()
{
    return orientation;
}

//valor da velocidade do chute
void Robot::setKick ( double newKick )
{
    kick = newKick;
}

double Robot::getKick ()
{
    return kick;
}

//seta a velocidade do driblador
void Robot::setDribble ( double newDribble )
{
    dribble = newDribble;
}

double Robot::getDribble ()
{
    return dribble;
}

void Robot::setRobotBehavior(RobotBehavior *_robotBehavior)
{
    robotBehavior = _robotBehavior;
}

RobotBehavior *Robot::getRobotBehavior()
{
    return robotBehavior;
}

Behavior Robot::getBehavior()
{
    return robotBehavior->getBehavior();
}

void Robot::resetRobot ()
{
    setIsOn ( false );
    setKick ( 0 );
    setDribble ( 0 );
    setTarget ( NULL );
    setGoingToBall ( false );
}

//retorna o ângulo necessário para o robô girar e ficar direcionado para o alvo EM RADIANOS
double Robot::calculateAngleTo ( Point *anotherPoint )
{
    double angle = this->getPosition()->calculateAngleTo( anotherPoint ) - this->getOrientation();
    UtilGame::adjustAngle( angle );
    return angle;
}

void Robot::setAngularVelocity ( double angVelocity )
{
    angularVelocity = angVelocity;
}

double Robot::getAngularVelocity ()
{
    return angularVelocity;
}

const int Robot::getId()
{
    return ID;
}

void Robot::setIsOn ( bool on )
{
    if ( on )
    {
        isOn = on;
        offTime1 = 0.0;
    }
    else
    {
        if ( offTime1 == 0.0 )
            offTime1 = UtilGame::countTime();
        else
        {
            unsigned int dt = UtilGame::countTime() - offTime1;
            //cout << "dt = " << dt << endl;
            //if ( dt >= 50000 )
            if ( dt >= 0 )
                isOn = on;
        }
    }
}

bool Robot::getIsOn ()
{
    return isOn;
}

string Robot::getBehaviorStr ()
{
    return behaviorStr [ this->getBehavior() ];
}

void Robot::addMark ( Robot *robot )
{
    isMarkedBy->push_back ( robot );
}

void Robot::clearIsMarkedBy ()
{
    isMarkedBy->clear();
}

vector <Robot *> *Robot::getIsMarkedBy()
{
    return isMarkedBy;
}

void Robot::setPossessionBall ( bool poss )
{
    possessionBall = poss;
}

bool Robot::getPossessionBall ()
{
    return possessionBall ;
}

void Robot::showInformation()
{
    cout << "ID = " << this->getId() <<
            "\nisReady = " << this->isReady() <<
            "\nbehavior = " << this->getBehaviorStr() <<
            "\n\ndistances [ DIR_FORWARD ] = " << distances [ DIR_FORWARD ] <<
            "\ndistances [ DIR_LEFT ] = " << distances [ DIR_LEFT ] <<
            "\ndistances [ DIR_RIGHT ] = " << distances [ DIR_RIGHT ] <<
            endl;
    cout << "Position: ";
    this->getPosition()->showInformation();
    cout << "PosToGo: ";
    this->getPosToGo()->showInformation();
}

void Robot::setPosToGo(Point *pos)
{
    posToGo = pos;
}

Point *Robot::getPosToGo()
{
    return posToGo;
}

bool Robot::isReady()
{
    return ( this->getPosition()->getDistanceTo ( this->posToGo ) < ROBOT_RADIUS &&
             this->getAngularVelocity() == 0.0 &&
             this->getVelocity()->getModule() == 0.0 );
}

void Robot::setGoingToBall(bool go)
{
    goingToBall = go;
}

bool Robot::getGoingToBall()
{
    return goingToBall;
}

double *Robot::getDistances()
{
    return distances;
}

//target refere-se ao alvo para o qual o robo esta orientado
void Robot::setTarget(Point *_target)
{
    target = _target;
}

Point *Robot::getTarget()
{
    return target;
}

bool Robot::foundObstacle()
{
    for ( int i = 0; i < ROBOT_DISTANCES; i++ )
        if ( distances [ i ] <= robotBehavior->getObjFLie()->getRHigh() )
            return true;
    return false;
}

void Robot::execCommand()
{
    comSystem->sendCommand( this );
}

void Robot::calculateLinearVelocity()
{
    robotBehavior->mapLinearVelocity();
}

void Robot::calculateAngularVelocity()
{
    robotBehavior->mapAngularVelocity();
}

void Robot::calculateVelocities()
{
    //teste -> precisa verificar as outras configuracoes se nao estao prejudicadas
//    if ( !this->isReady() )
//    {
//        robotBehavior->calculateVelocities();
//    }

    robotBehavior->calculateVelocities();
}

void Robot::calculateDistances()
{
    if ( this->isReady() )
        return;
    distances [ DIR_FORWARD ] = robotBehavior->auxCalculateRobotDistances ( 0 );
    distances [ DIR_LEFT ] = robotBehavior->auxCalculateRobotDistances ( ANGLE_DIRECTION );
    distances [ DIR_RIGHT ] = robotBehavior->auxCalculateRobotDistances ( -ANGLE_DIRECTION );
}

void Robot::calculateKickZ()
{
    this->setDribble ( robotBehavior->calculateMapKickZ() );
}

void Robot::calculateKickWithZ()
{
    this->setKick ( robotBehavior->calculateMapKickWithZ() );
}

void Robot::calculateKick()
{
    this->setKick ( robotBehavior->calculateMapKick() );
}

void Robot::calculateKickZPartner()
{
    this->setDribble ( robotBehavior->calculateMapKickZPartner() );
}

void Robot::calculateKickWithZPartner()
{
    this->setKick ( robotBehavior->calculateMapKickWithZPartner() );
}

void Robot::calculateKickToPartner()
{
    this->setKick ( robotBehavior->calculateMapKickToPartner () );
}
